/**
 * MOTOR PWM CONNECTION
 * PTB0 A1, PTB1 A2 Green
 * PTB2 A1, PTB3 A2 Blue
 */

#include "MKL25Z4.h"                    // Device header

/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "uart.h"
#include "ledControl.h"
#include "motors.h"
#include "sound.h"

volatile uint32_t rx_data = 0;
volatile uint32_t led_count = 0;

int isRunning = -1;

uint32_t GREEN_LEDS_STRIP[] = {GREEN_LED_1, GREEN_LED_2, GREEN_LED_3, GREEN_LED_4, 
        GREEN_LED_5, GREEN_LED_6, GREEN_LED_7, GREEN_LED_8};

osEventFlagsId_t isReceivingData;
osEventFlagsId_t shouldForward;
osEventFlagsId_t shouldReverse;
osEventFlagsId_t shouldLeft;
osEventFlagsId_t shouldRight;
osEventFlagsId_t shouldStop;
        
const osThreadAttr_t thread_attr = {    // unused
    .priority = osPriorityAboveNormal
};      


void UART2_IRQHandler(void) {
    NVIC_ClearPendingIRQ(UART2_IRQn);
    
    if (UART2->S1 & UART_S1_RDRF_MASK) {
        // UART RX received all the bits
        rx_data = UART2_D;
    }
    
    //Clear INT Flag
    PORTE->ISFR |= MASK(UART_RX_PORTE23);
}
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void red_led_thread(void *argument) {    
    for (;;) {
        if (isRunning == 0) {
            ledControl(RED_LEDS, LED_ON);
            osDelay(250);
            ledControl(RED_LEDS, LED_OFF);
            osDelay(250);
        } else if (isRunning == 1) {
            ledControl(RED_LEDS, LED_ON);
            osDelay(500);
            ledControl(RED_LEDS, LED_OFF);
            osDelay(500);
        }
    }
}

void green_led_thread(void *argument) {
    for (;;) {
        if (BLUETOOTH_CONNECTED_MASK(rx_data) == BLUETOOTH_CONNECTED) {
            for (int i = 0; i < 3; i++) {
                on2GreenLeds();
                osDelay(250);
                offAllLeds();
                osDelay(250);
            }
            rx_data = 0;
        }
                    
        if (isRunning == 0) {
            onAllGreenLeds();
        } else if (isRunning == 1) {
            ledControl(GREEN_LEDS_STRIP[led_count], LED_ON);
            osDelay(50);
            offAllGreenLeds();

            led_count = ((led_count == 7) ? 0 : led_count + 1);
        }
    }
}

void tMotor_Forward(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldForward, 0x01, osFlagsNoClear, osWaitForever);
        isRunning = 1;
        move(FORWARD);
    }
}

void tMotor_Reverse(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldReverse, 0x01, osFlagsNoClear, osWaitForever);
        isRunning = 1;
        move(REVERSE);
    }
}

void tMotor_Left(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldLeft, 0x01, osFlagsNoClear, osWaitForever);
        isRunning = 1;
        move(LEFT);
    }
}

void tMotor_Right(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldRight, 0x01, osFlagsNoClear, osWaitForever);
        isRunning = 1;
        move(RIGHT);
    }
}

void tMotor_Stop(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldStop, 0x01, osFlagsWaitAny , osWaitForever);
        isRunning = 0;
        move(STOP);
    }
}

void tBrain(void *argument) {
    for (;;) {
        if (rx_data == 32) {
            move(STOP);
        }
        
        switch (MOVEMENT_BUTTON_MASK(rx_data)) {
            case UP_BUTTON_PRESSED:
                osEventFlagsSet(shouldForward, 0x01);
                break;
            case DOWN_BUTTON_PRESSED:
                osEventFlagsSet(shouldReverse, 0x01);
                break;
            case LEFT_BUTTON_PRESSED:
                osEventFlagsSet(shouldLeft, 0x01);
                break;
            case RIGHT_BUTTON_PRESSED:
                osEventFlagsSet(shouldRight, 0x01);
                break;
            case UP_BUTTON_RELEASED:
                osEventFlagsClear(shouldForward, 0x01);
                osEventFlagsSet(shouldStop, 0x01);
                break;
            case DOWN_BUTTON_RELEASED:
                osEventFlagsClear(shouldReverse, 0x01);
                osEventFlagsSet(shouldStop, 0x01);
                break;
            case LEFT_BUTTON_RELEASED:
                osEventFlagsClear(shouldLeft, 0x01);
                osEventFlagsSet(shouldStop, 0x01);
                break;
            case RIGHT_BUTTON_RELEASED:
                osEventFlagsClear(shouldRight, 0x01);
                osEventFlagsSet(shouldStop, 0x01);
                break;
        }
    }
}

void tSound_opening(void *argument) {
    for (;;) {
        opening_sound();
    }
}

void tSound_running(void *argument) {
    for (;;) {
        running_sound();
    }
}

void tSound_ending(void *argument) {
    for (;;) {
        ending_sound();
    }
}

int main (void) {
 
    // System Initialization
    SystemCoreClockUpdate();
    initUART2();
    initMotors();
    initSound();
    initLed();
    
    offAllLeds();
    move(STOP);
 
    osKernelInitialize();                 // Initialize CMSIS-RTOS
    
    isReceivingData = osEventFlagsNew(NULL);
    
    shouldForward = osEventFlagsNew(NULL);
    shouldReverse = osEventFlagsNew(NULL);
    shouldLeft = osEventFlagsNew(NULL);
    shouldRight = osEventFlagsNew(NULL);
    shouldStop = osEventFlagsNew(NULL);
    
    osThreadNew(tBrain, NULL, NULL);
    
    osThreadNew(green_led_thread, NULL, NULL);    // Create application main thread
    osThreadNew(red_led_thread, NULL, NULL);    // Create application main thread
    
    osThreadNew(tMotor_Forward, NULL, NULL);
    osThreadNew(tMotor_Reverse, NULL, NULL);
    osThreadNew(tMotor_Left, NULL, NULL);
    osThreadNew(tMotor_Right, NULL, NULL);
    osThreadNew(tMotor_Stop, NULL, NULL);
    
    osKernelStart();                      // Start thread execution
    for (;;) {}

}
