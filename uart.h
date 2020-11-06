#ifndef UART_H_
#define UART_H_

#define UART_RX_PORTE23             23     // UART Receiver
#define BAUD_RATE                   9600
#define UART2_INT_PRIO              128
#define Q_SIZE						5

#define BLUETOOTH_CONNECTED         1
#define BLUETOOTH_CONNECTED_MASK(x) (x & 0x01)

#define MOVEMENT_BUTTON_MASK(x)     ((x & 0x1E) >> 1)

#define UP_BUTTON_PRESSED           1
#define UP_BUTTON_RELEASED          2

#define DOWN_BUTTON_PRESSED         3
#define DOWN_BUTTON_RELEASED        4

#define LEFT_BUTTON_PRESSED         5
#define LEFT_BUTTON_RELEASED        6

#define RIGHT_BUTTON_PRESSED        7
#define RIGHT_BUTTON_RELEASED       8

#define ALL_BUTTON_RELEASED         9

#define FINISH_BUTTON_PRESSED       1
#define FINISH_BUTTON_PRESS_MASK(x) ((x & 0x20) >> 5)

typedef struct {
	unsigned char Data[Q_SIZE];
	unsigned int Head;
	unsigned int Tail;
	unsigned int size;
} Q_T;

/* Init UART2 */
void initUART2(void);

void initQueue(Q_T*);
int Q_Empty(Q_T*);
int Q_Full(Q_T*);
int Q_Enqueue(Q_T*,	unsigned char);
unsigned char Q_Dequeue(Q_T*);

#endif
