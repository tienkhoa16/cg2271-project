#ifndef LEDCONTROL_H
#include "MKL25Z4.h"

#define LEDCONTROL_H
#define MASK(x)         (1 << (x)) 

//Define pin numbers
#define GREEN_LED_1  		  11  // PortC Pin 11 
#define GREEN_LED_2           10  // PortC Pin 10
#define GREEN_LED_3           6   // PortC Pin 6
#define GREEN_LED_4           5   // PortC Pin 5
#define GREEN_LED_5           4   // PortC Pin 4
#define GREEN_LED_6           3   // PortC Pin 3
#define GREEN_LED_7           0   // PortC Pin 0
#define GREEN_LED_8           7   // PortC Pin 7
#define RED_LEDS              12  // PortC Pin 12
#define ALL_GREEN_LEDS_MASK   0x00000CF9
#define LED_ON                1
#define LED_OFF               0

// Delay for different modes
#define MOVING_DELAY		    500		// 500 ms
#define STATIONARY_DELAY 		250		// 250 ms

void initLed(void);
void offAllLeds(void);
void ledControl(uint32_t, uint32_t);
void on2GreenLeds(void);
void offAllGreenLeds(void);
void onAllGreenLeds(void);
        
#endif
