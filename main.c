/**
 * MOTOR PWM CONNECTION
 * PTB0 A1, PTB1 A2 Green
 * PTB2 A1, PTB3 A2 Blue
 */

#include "MKL25Z4.h"                    // Device header

int counter = 0;
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
uint32_t GREEN_LEDS_STRIP[8] = {GREEN_LED_1, GREEN_LED_2, GREEN_LED_3, GREEN_LED_4, 
        GREEN_LED_5, GREEN_LED_6, GREEN_LED_7, GREEN_LED_8};

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
    int isRunning = -1;
    
    for (;;) {
        if (BUTTON_RELEASED_MASK(rx_data) == BUTTON_RELEASED) {
            isRunning = 0;
        }

        if (UP_BUTTON_PRESSED_MASK(rx_data) == UP_BUTTON_PRESSED || 
                    LEFT_BUTTON_PRESSED_MASK(rx_data) == LEFT_BUTTON_PRESSED || 
                    RIGHT_BUTTON_PRESSED_MASK(rx_data) == RIGHT_BUTTON_PRESSED || 
                    DOWN_BUTTON_PRESSED_MASK(rx_data) == DOWN_BUTTON_PRESSED) {
            isRunning = 1;
        }
                    
        if (isRunning == 0) {
            ledControl(RED_LEDS, LED_ON);
            osDelay(250);
            ledControl(RED_LEDS, LED_OFF);
            osDelay(250);
        } 
        if (isRunning == 1) {
            ledControl(RED_LEDS, LED_ON);
            osDelay(500);
            ledControl(RED_LEDS, LED_OFF);
            osDelay(500);
        }
    }
}

void green_led_thread(void *argument) {
    int isRunning = -1;
    
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

        if (BUTTON_RELEASED_MASK(rx_data) == BUTTON_RELEASED) {
            isRunning = 0;
        }

        if (UP_BUTTON_PRESSED_MASK(rx_data) == UP_BUTTON_PRESSED || 
                    LEFT_BUTTON_PRESSED_MASK(rx_data) == LEFT_BUTTON_PRESSED || 
                    RIGHT_BUTTON_PRESSED_MASK(rx_data) == RIGHT_BUTTON_PRESSED || 
                    DOWN_BUTTON_PRESSED_MASK(rx_data) == DOWN_BUTTON_PRESSED) {
            isRunning = 1;
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

void tMotor_Left(void *argument) {
	for (;;) {
		if (LEFT_BUTTON_PRESSED_MASK(rx_data) == LEFT_BUTTON_PRESSED) {
			do {
				move(LEFT);
			} while (BUTTON_RELEASED_MASK(rx_data) != BUTTON_RELEASED);
		}
	}
}

void tMotor_Right(void *argument) {
	for (;;) {
		if (RIGHT_BUTTON_PRESSED_MASK(rx_data) == RIGHT_BUTTON_PRESSED) {
			do {
				move(RIGHT);
			} while (BUTTON_RELEASED_MASK(rx_data) != BUTTON_RELEASED);
		}
	}
}

void tMotor_Forward(void *argument) {
	for (;;) {
		if (UP_BUTTON_PRESSED_MASK(rx_data) == UP_BUTTON_PRESSED) {
			do {
				move(FORWARD);
			} while (BUTTON_RELEASED_MASK(rx_data) != BUTTON_RELEASED);
		}
	}
}

void tMotor_Reverse(void *argument) {
	for (;;) {
		if (DOWN_BUTTON_PRESSED_MASK(rx_data) == DOWN_BUTTON_PRESSED) {
			do {
				move(REVERSE);
			} while (BUTTON_RELEASED_MASK(rx_data) != BUTTON_RELEASED);
		}
	}
}

void tMotor_Stop(void *argument) {
	for (;;) {
		if (BUTTON_RELEASED_MASK(rx_data) == BUTTON_RELEASED) {
			move(STOP);
			osDelay(10);
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

static void delay(volatile uint32_t nof) {
	while(nof!=0) {
		__asm("NOP");
		nof--;
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
 
    osKernelInitialize();                 // Initialize CMSIS-RTOS
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