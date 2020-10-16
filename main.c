#include "MKL25Z4.h"                    // Device header
#include "uart.h"
#include "ledControl.h"

volatile uint8_t led_count = 0;
uint32_t GREEN_LEDS_STRIP[8] = {GREEN_LED_1, GREEN_LED_2, GREEN_LED_3, GREEN_LED_4, 
        GREEN_LED_5, GREEN_LED_6, GREEN_LED_7, GREEN_LED_8};

/* Delay Function */
static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

void UART2_IRQHandler(void) {
    NVIC_ClearPendingIRQ(UART2_IRQn);
    
    uint8_t rx_data = UART2_D;

    if (UART2->S1 & UART_S1_RDRF_MASK) {
        if (BLUETOOTH_CONNECTED_MASK(rx_data) == BLUETOOTH_CONNECTED) {
            for (int i = 0; i < 3; i++) {
                on2GreenLeds();
                delay(0x120000);
                offAllLeds();
                delay(0x120000);
            }
        } 

        if (BUTTON_RELEASED_MASK(rx_data) == BUTTON_RELEASED) {
            onAllGreenLeds();
        }

        while (UP_BUTTON_PRESSED_MASK(rx_data) == UP_BUTTON_PRESSED || 
                    LEFT_BUTTON_PRESSED_MASK(rx_data) == LEFT_BUTTON_PRESSED || 
                    RIGHT_BUTTON_PRESSED_MASK(rx_data) == RIGHT_BUTTON_PRESSED || 
                    DOWN_BUTTON_PRESSED_MASK(rx_data) == DOWN_BUTTON_PRESSED) {

            ledControl(GREEN_LEDS_STRIP[led_count], LED_ON);
            delay(0x80000);
            offAllGreenLeds();

            led_count = ((led_count == 7) ? 0 : led_count + 1);

            rx_data = UART2_D;
        }
    }
    
    //Clear INT Flag
    PORTE->ISFR |= MASK(UART_RX_PORTE23);
}

int main(void) {
    SystemCoreClockUpdate();
    initUART2();
    initLed();
    
    offAllLeds();
    while (1) {

    }
    
}
