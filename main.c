/**
 * MOTOR PWM CONNECTION
 * PTB0, PTB1 A1 A2 Green
 * PTB2, PTB3 A1 A2 Blue
 */

#include "MKL25Z4.h"                    // Device header
#include "sound.h"

int main(void)
{
	SystemCoreClockUpdate();
	initSound();
	
	while(1) {
		ending_sound();
	}
}
