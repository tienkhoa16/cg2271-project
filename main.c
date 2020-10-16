/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "uart.h"
#include "ledControl.h"

volatile uint32_t rx_data = 0;
volatile uint32_t led_count = 0;
uint32_t GREEN_LEDS_STRIP[8] = {GREEN_LED_1, GREEN_LED_2, GREEN_LED_3, GREEN_LED_4, 
        GREEN_LED_5, GREEN_LED_6, GREEN_LED_7, GREEN_LED_8};

void UART2_IRQHandler(void) {
    NVIC_ClearPendingIRQ(UART2_IRQn);
    
    if (UART2->S1 & UART_S1_RDRF_MASK) {
        rx_data = UART2_D;
    }
    
    //Clear INT Flag
    PORTE->ISFR |= MASK(UART_RX_PORTE23);
}
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void tLed(void *argument) {
    for (;;) {
        if (BLUETOOTH_CONNECTED_MASK(rx_data) == BLUETOOTH_CONNECTED) {
            for (int i = 0; i < 3; i++) {
                on2GreenLeds();
                osDelay(500);
                offAllLeds();
                osDelay(500);
            }
            rx_data = 0;
        } 

        if (BUTTON_RELEASED_MASK(rx_data) == BUTTON_RELEASED) {
            onAllGreenLeds();
        }

        if (UP_BUTTON_PRESSED_MASK(rx_data) == UP_BUTTON_PRESSED || 
                    LEFT_BUTTON_PRESSED_MASK(rx_data) == LEFT_BUTTON_PRESSED || 
                    RIGHT_BUTTON_PRESSED_MASK(rx_data) == RIGHT_BUTTON_PRESSED || 
                    DOWN_BUTTON_PRESSED_MASK(rx_data) == DOWN_BUTTON_PRESSED) {

            ledControl(GREEN_LEDS_STRIP[led_count], LED_ON);
            osDelay(50);
            offAllGreenLeds();

            led_count = ((led_count == 7) ? 0 : led_count + 1);
        }
    }
}
 
int main (void) {
 
    // System Initialization
    SystemCoreClockUpdate();
    initUART2();
    initLed();
    offAllLeds();
 
    osKernelInitialize();                 // Initialize CMSIS-RTOS
    osThreadNew(tLed, NULL, NULL);    // Create application main thread
    osKernelStart();                      // Start thread execution
    for (;;) {}
}
