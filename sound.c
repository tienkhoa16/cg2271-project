#include "MKL25Z4.h"
#include "sound.h"
#include "cmsis_os2.h"

#define RUNNING_CNT     sizeof(running_notes)/sizeof(running_notes[0])
#define OPENING_CNT     sizeof(opening_notes)/sizeof(opening_notes[0])
#define ENDING_CNT      sizeof(ending_notes)/sizeof(ending_notes[0])
#define FREQ_2_MOD(x)   (375000/x)

// Glory glory ManUtd
int ending_notes[] = {
	A3, 0, G3, FS3, A3, D4, E4, FS4, 0, D4, 0,
	B3, 0, C4, D4, C4, D4, C4, A3, 0, FS3, 0,
	A3, 0, G3, FS3, A3, D4, E4, FS4, 0, D4, 0,
	D4, D4, E4, 0, E4, 0, D4, 0, CS4, 0, D4, 0, D4, 0, D4, 0
};

int ending_durations[] = {
	250, 5, 100, 100, 100, 100, 125, 300, 5, 250, 75,
	250, 5, 75, 150, 50, 150, 75, 300, 5, 250, 75,
	250, 5, 100, 100, 100, 100, 125, 300, 5, 250, 50,
	125, 125, 175, 50, 175, 50, 150, 10, 150, 25, 150, 75, 150, 75, 150, 150
};

// Pirates of the Carribean
int running_notes[] = {
	E4, G4, A4, A4, 0, 
	A4, B4, C5, C5, 0, 
	C5, D5, B4, B4, 0,
	A4, G4, A4, 0,

	E4, G4, A4, A4, 0, 
	A4, B4, C5, C5, 0, 
	C5, D5, B4, B4, 0,
	A4, G4, A4, 0,

	E4, G4, A4, A4, 0, 
	A4, C5, D5, D5, 0, 
	D5, E5, F5, F5, 0,
	E5, D5, E5, A4, 0,

	A4, B4, C5, C5, 0, 
	D5, E5, A4, 0, 
	A4, C5, B4, B4, 0,
	C5, A4, B4, 0,

	A4, A4, 
	//Repeat of first part
	A4, B4, C5, C5, 0, 
	C5, D5, B4, B4, 0,
	A4, G4, A4, 0,

	E4, G4, A4, A4, 0, 
	A4, B4, C5, C5, 0, 
	C5, D5, B4, B4, 0,
	A4, G4, A4, 0,

	E4, G4, A4, A4, 0, 
	A4, C5, D5, D5, 0, 
	D5, E5, F5, F5, 0,
	E5, D5, E5, A4, 0,

	A4, B4, C5, C5, 0, 
	D5, E5, A4, 0, 
	A4, C5, B4, B4, 0,
	C5, A4, B4, 0,
	//End of Repeat

	E5, 0, 0, F5, 0, 0,
	E5, E5, 0, G5, 0, E5, D5, 0, 0,
	D5, 0, 0, C5, 0, 0,
	B4, C5, 0, B4, 0, A4,

	E5, 0, 0, F5, 0, 0,
	E5, E5, 0, G5, 0, E5, D5, 0, 0,
	D5, 0, 0, C5, 0, 0,
	B4, C5, 0, B4, 0, A4, 0
};

int running_durations[] = {         
	125, 125, 250, 125, 125, 
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 375, 125, 

	125, 125, 250, 125, 125, 
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 375, 125, 

	125, 125, 250, 125, 125, 
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 125, 250, 125,

	125, 125, 250, 125, 125, 
	250, 125, 250, 125, 
	125, 125, 250, 125, 125,
	125, 125, 375, 375,

	250, 125,
	//Rpeat of First Part
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 375, 125, 

	125, 125, 250, 125, 125, 
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 375, 125, 

	125, 125, 250, 125, 125, 
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 125, 250, 125,

	125, 125, 250, 125, 125, 
	250, 125, 250, 125, 
	125, 125, 250, 125, 125,
	125, 125, 375, 375,
	//End of Repeat

	250, 125, 375, 250, 125, 375,
	125, 125, 125, 125, 125, 125, 125, 125, 375,
	250, 125, 375, 250, 125, 375,
	125, 125, 125, 125, 125, 500,

	250, 125, 375, 250, 125, 375,
	125, 125, 125, 125, 125, 125, 125, 125, 375,
	250, 125, 375, 250, 125, 375,
	125, 125, 125, 125, 125, 500, 100
};

// NOKIA RINGTONE
int opening_notes[] = {
	E5, D5, FS4, GS4, 0,
	CS5, B4, D4, E4, 0,
	B4, A4, CS4, E4, A4, 0
};

int opening_durations[] = {
	125, 125, 125, 150, 10,
	125, 125, 125, 125, 10,
	125, 125, 125, 125, 200, 150
};

/* Init PWM Module */
void initSound(void) 
{
	// Enable Clock for port PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// Configure mode 3 for PWM pin operation (Timer 0, CH2)
	PORTE->PCR[PTE_PIN29] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE_PIN29] |= PORT_PCR_MUX(3);
	
	// Enable Clock for Timer 1
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	// Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // MCGFLLCLK clock or MCGPLLCLK/2
	
	// Edge-aligned PWM
	// Update SnC register: CMOD = 01, PS = 111 (Prescalar: 128)
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	// Enable PWM on TPM0_Ch2
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK));
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void running_sound(void) {
    for (int i = 0; i < RUNNING_CNT; i++) {
        TPM0->MOD = FREQ_2_MOD(running_notes[i]);
        TPM0_C2V = 0;
        osDelay(running_durations[i]);
    }
}

void ending_sound(void) {
	for (int i = 0; i < ENDING_CNT; i++) {
		TPM0->MOD = FREQ_2_MOD(ending_notes[i]);
		TPM0_C2V = (FREQ_2_MOD(ending_notes[i])) / 2;
		osDelay(ending_durations[i]);
	}
    TPM0_C2V = 0;
    SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK;
}

void opening_sound(void) {
	for (int i = 0; i < OPENING_CNT; i++) {
		TPM0->MOD = FREQ_2_MOD(opening_notes[i]);
		TPM0_C2V = (FREQ_2_MOD(opening_notes[i])) / 2;
		osDelay(opening_durations[i]);
	}
}

void stop_sound(void) {
	TPM0_C2V = 0;
    osDelay(100);
}
