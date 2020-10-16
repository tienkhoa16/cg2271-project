#ifndef UART_H_
#define UART_H_

#define UART_RX_PORTE23 23     // UART Receiver
#define BAUD_RATE 9600
#define UART2_INT_PRIO 128

#define BLUETOOTH_CONNECTED 1
#define BLUETOOTH_CONNECTED_MASK(x) (x & 0x01)

#define BUTTON_RELEASED 32
#define BUTTON_RELEASED_MASK(x) (x & 0x20)

#define UP_BUTTON_PRESSED 2
#define UP_BUTTON_PRESSED_MASK(x) (x & 0x02)

#define LEFT_BUTTON_PRESSED 4
#define LEFT_BUTTON_PRESSED_MASK(x) (x & 0x04)

#define RIGHT_BUTTON_PRESSED 8
#define RIGHT_BUTTON_PRESSED_MASK(x) (x & 0x08)

#define DOWN_BUTTON_PRESSED 16
#define DOWN_BUTTON_PRESSED_MASK(x) (x & 0x10)



/* Init UART2 */
void initUART2(void);

#endif
