#ifndef UART_H_
#define UART_H_

#define UART_RX_PORTE23             23     // UART Receiver
#define BAUD_RATE                   9600
#define UART2_INT_PRIO              128    // Interrupt priority

#define BLUETOOTH_CONNECTED         1
#define BLUETOOTH_CONNECTED_MASK(x) (x & 0x01)

#define MOVEMENT_BUTTON_MASK(x)     ((x & 0x1E) >> 1)

#define UP_BUTTON_PRESSED           0x01
#define UP_BUTTON_RELEASED          0x02

#define DOWN_BUTTON_PRESSED         0x03
#define DOWN_BUTTON_RELEASED        0x04

#define LEFT_BUTTON_PRESSED         0x05
#define LEFT_BUTTON_RELEASED        0x06

#define RIGHT_BUTTON_PRESSED        0x07
#define RIGHT_BUTTON_RELEASED       0x08

#define ALL_BUTTON_RELEASED         0x09

#define FINISH_BUTTON_PRESSED       0x01
#define FINISH_BUTTON_PRESS_MASK(x) ((x & 0x20) >> 5)

/* Init UART2 */
void initUART2(void);

#endif
