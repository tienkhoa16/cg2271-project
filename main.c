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
