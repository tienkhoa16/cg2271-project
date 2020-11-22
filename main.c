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

// Green LEDs array
const uint32_t GREEN_LEDS_STRIP[] = {GREEN_LED_1, GREEN_LED_2, GREEN_LED_3, GREEN_LED_4, 
        GREEN_LED_5, GREEN_LED_6, GREEN_LED_7, GREEN_LED_8};

// Received data from BT06 module
volatile uint32_t rx_data = 0;

// Flags for motors movement
osEventFlagsId_t shouldForward;
osEventFlagsId_t shouldReverse;
osEventFlagsId_t shouldLeft;
osEventFlagsId_t shouldRight;
osEventFlagsId_t shouldStop;

// Flags for audio playing
osEventFlagsId_t bluetoothConnected;
osEventFlagsId_t shouldPlayRunning;
osEventFlagsId_t shouldPlayEnding;

/* Interupt for capturing serial data */
void UART2_IRQHandler(void) {
    NVIC_ClearPendingIRQ(UART2_IRQn);
    
    if (UART2->S1 & UART_S1_RDRF_MASK) {
        rx_data = UART2_D;
    }
    
    //Clear INT Flag
    PORTE->ISFR |= MASK(UART_RX_PORTE23);
}

/* Returns if robot is running or not */
int isRunning(void) {
	return osEventFlagsGet(shouldForward) == 0x01 ||
		osEventFlagsGet(shouldLeft) == 0x01 ||
		osEventFlagsGet(shouldReverse) == 0x01 ||
		osEventFlagsGet(shouldRight) == 0x01;
}
 
/*----------------------------------------------------------------------------
 * Application main threads
 *---------------------------------------------------------------------------*/

/* Red LEDs thread */
void tLed_red(void *argument) {    
	osEventFlagsWait(bluetoothConnected, 0x01, osFlagsWaitAny, osWaitForever);
	for (;;) {
        if (isRunning() == 0) {
            ledControl(RED_LEDS, LED_ON);
            osDelay(STATIONARY_DELAY);
            ledControl(RED_LEDS, LED_OFF);
            osDelay(STATIONARY_DELAY);
        } else if (isRunning() == 1) {
            ledControl(RED_LEDS, LED_ON);
            osDelay(MOVING_DELAY);
            ledControl(RED_LEDS, LED_OFF);
            osDelay(MOVING_DELAY);
        }
    }
}

/* Green LEDs thread */
void tLed_green(void *argument) {
    osEventFlagsWait(bluetoothConnected, 0x01, osFlagsWaitAny, osWaitForever);
    for (int i = 0; i < 3; i++) {
        on2GreenLeds();
        osDelay(STATIONARY_DELAY);
        offAllLeds();
        osDelay(STATIONARY_DELAY);
    }
    
    uint32_t led_count = 0;
    for (;;) {
        if (isRunning() == 0) {
            onAllGreenLeds();
        } else if (isRunning() == 1) {
            ledControl(GREEN_LEDS_STRIP[led_count], LED_ON);
            osDelay(50);
            offAllGreenLeds();

            led_count = ((led_count == 7) ? 0 : led_count + 1);
        }
    }
}

/* Moving forward thread */
void tMotor_Forward(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldForward, 0x01, osFlagsNoClear, osWaitForever);
        move(FORWARD);
    }
}

/* Moving reverse thread */
void tMotor_Reverse(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldReverse, 0x01, osFlagsNoClear, osWaitForever);
        move(REVERSE);
    }
}

/* Turning left thread */
void tMotor_Left(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldLeft, 0x01, osFlagsNoClear, osWaitForever);
        move(LEFT);
    }
}

/* Turning right thread */
void tMotor_Right(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldRight, 0x01, osFlagsNoClear, osWaitForever);
        move(RIGHT);
    }
}

/* Stop movement thread */
void tMotor_Stop(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldStop, 0x01, osFlagsWaitAny , osWaitForever);
        move(STOP);
    }
}

/* Thread for decoding serial data and performing necessary actions */
void tBrain(void *argument) {
    for (;;) {
        if (BLUETOOTH_CONNECTED_MASK(rx_data) == BLUETOOTH_CONNECTED) {
            osEventFlagsSet(bluetoothConnected, 0x01);
        }

        if (FINISH_BUTTON_PRESS_MASK(rx_data) == FINISH_BUTTON_PRESSED) {
            osEventFlagsSet(shouldStop, 0x01);
            osEventFlagsClear(shouldPlayRunning, 0x01);
            osEventFlagsSet(shouldPlayEnding, 0x01);
        }
		
        switch (MOVEMENT_BUTTON_MASK(rx_data)) {
            case UP_BUTTON_PRESSED:
                osEventFlagsSet(shouldForward, 0x01);
                osEventFlagsSet(shouldPlayRunning, 0x01);
                break;
            case DOWN_BUTTON_PRESSED:
                osEventFlagsSet(shouldReverse, 0x01);
                osEventFlagsSet(shouldPlayRunning, 0x01);
                break;
            case LEFT_BUTTON_PRESSED:
                osEventFlagsSet(shouldLeft, 0x01);
                osEventFlagsSet(shouldPlayRunning, 0x01);
                break;
            case RIGHT_BUTTON_PRESSED:
                osEventFlagsSet(shouldRight, 0x01);
                osEventFlagsSet(shouldPlayRunning, 0x01);
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
            case ALL_BUTTON_RELEASED:
                osEventFlagsClear(shouldForward, 0x01);
                osEventFlagsClear(shouldReverse, 0x01);
                osEventFlagsClear(shouldLeft, 0x01);
                osEventFlagsClear(shouldRight, 0x01);
                osEventFlagsSet(shouldStop, 0x01);
                break;
        }
    }
}

/* Thread for playing sound when bluetooth connection is established */
void tSound_opening(void *argument) {
    osEventFlagsWait(bluetoothConnected, 0x01, osFlagsWaitAny, osWaitForever);
    opening_sound();
}

/* Thread for playing sound while the robot is in the challenge run */
void tSound_running(void *argument) {
    for (;;) {
		for (int i = 0; i < RUNNING_COUNT; i++) {
			osEventFlagsWait(shouldPlayRunning, 0x01, osFlagsNoClear, osWaitForever);
			stop_sound();
			running_sound(i);
		}
	}
}

/* Thread for playing sound when the robot finishes the challenge run */
void tSound_ending(void *argument) {
    for (;;) {
        osEventFlagsWait(shouldPlayEnding, 0x01, osFlagsNoClear, osWaitForever);
        osEventFlagsClear(shouldPlayRunning, 0x01);
        stop_sound();
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
    
    // Turn off LEDs
    offAllLeds();
    
    // Stop all motors
    move(STOP);
    
    // Stop the buzzer
    stop_sound();
	
    // Initialize CMSIS-RTOS
    osKernelInitialize();
    
    // Create new osEventFlags
    bluetoothConnected = osEventFlagsNew(NULL);
    
    shouldPlayRunning = osEventFlagsNew(NULL);
    shouldPlayEnding = osEventFlagsNew(NULL);
    
    shouldForward = osEventFlagsNew(NULL);
    shouldReverse = osEventFlagsNew(NULL);
    shouldLeft = osEventFlagsNew(NULL);
    shouldRight = osEventFlagsNew(NULL);
    shouldStop = osEventFlagsNew(NULL);
    
    // Create threads in the system
    osThreadNew(tBrain, NULL, NULL);
    
    osThreadNew(tLed_green, NULL, NULL); 
    osThreadNew(tLed_red, NULL, NULL); 
    
    osThreadNew(tMotor_Forward, NULL, NULL);
    osThreadNew(tMotor_Reverse, NULL, NULL);
    osThreadNew(tMotor_Left, NULL, NULL);
    osThreadNew(tMotor_Right, NULL, NULL);
    osThreadNew(tMotor_Stop, NULL, NULL);
    
    osThreadNew(tSound_opening, NULL, NULL);
    osThreadNew(tSound_running, NULL, NULL);
    osThreadNew(tSound_ending, NULL, NULL);
    
    // Start thread execution
    osKernelStart();
    for (;;) {}
}
