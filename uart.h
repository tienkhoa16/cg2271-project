#ifndef UART_H_
#define UART_H_

#define UART_RX_PORTE23             23     // UART Receiver
#define BAUD_RATE                   9600
#define UART2_INT_PRIO              128

#define BLUETOOTH_CONNECTED         1
#define BLUETOOTH_CONNECTED_MASK(x) (x & 0x01)

#define MOVEMENT_BUTTON_MASK(x)     (x & 0x1E)

#define UP_BUTTON_PRESSED           2
#define UP_BUTTON_RELEASED          4

#define DOWN_BUTTON_PRESSED         6
#define DOWN_BUTTON_RELEASED        8

#define LEFT_BUTTON_PRESSED         10
#define LEFT_BUTTON_RELEASED        12

#define RIGHT_BUTTON_PRESSED        14
#define RIGHT_BUTTON_RELEASED       16

#define FINISH_BUTTON_PRESSED       32
#define FINISH_BUTTON_PRESS_MASK(x) (x & 0x20)

/* Init UART2 */
void initUART2(void);

#endif
