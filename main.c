/**
 * MOTOR PWM CONNECTION
 * PTB0 A1, PTB1 A2 Green
 * PTB2 A1, PTB3 A2 Blue
 */

#include "MKL25Z4.h"                    // Device header
#include "motors.h"

int counter = 0;

static void delay(volatile uint32_t nof) {
	while(nof!=0) {
		__asm("NOP");
		nof--;
	}
}

int main(void) {
    SystemCoreClockUpdate();
    initMotors();
	
    while (1) {
		move(counter);
		delay(5000000);
		counter = (counter < 4) ? counter + 1 : 0;
    }
}
