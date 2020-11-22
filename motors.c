#include "MKL25Z4.h"                    // Device header
#include "motors.h"

/* Initializes motors */
void initMotors(void) {
	// Enable Clock for PORTD
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// TPM1_CH0
	PORTB->PCR[RIGHT_MOTOR_FORWARD] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RIGHT_MOTOR_FORWARD] |= PORT_PCR_MUX(3);
	
	// TPM1_CH1
	PORTB->PCR[RIGHT_MOTOR_REVERSE] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RIGHT_MOTOR_REVERSE] |= PORT_PCR_MUX(3);

	// TPM2_CH0
	PORTB->PCR[LEFT_MOTOR_FORWARD] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LEFT_MOTOR_FORWARD] |= PORT_PCR_MUX(3);

	// TPM2_CH1
	PORTB->PCR[LEFT_MOTOR_REVERSE] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LEFT_MOTOR_REVERSE] |= PORT_PCR_MUX(3);
	
	// Enable Clock gating for Timer 1 and Timer 2
	SIM->SCGC6 |= (SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK);
	
	// Select Clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// Set MOD to adjust 50Hz frequency
    TPM1_MOD = 7500;
    TPM2_MOD = 7500;
    
    // Set CMOD to 01 and prescaler to 128 for timer 1 and 2
    TPM1_SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK);
    TPM1_SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
    TPM2_SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK);
    TPM2_SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
    
    // Enable up counting for timer 1 and 2
    TPM1_SC &= ~TPM_SC_CPWMS_MASK;
    TPM2_SC &= ~TPM_SC_CPWMS_MASK;
    
    // Set Edge-aligned PWM, clear Output on match for timer 1 channel 0
    TPM1_C0SC &= (TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
    TPM1_C0SC |= (TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1));
    
    // Set Edge-aligned PWM, clear Output on match for timer 1 channel 1
    TPM1_C1SC &= (TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
    TPM1_C1SC |= (TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1));
    
    // Set Edge-aligned PWM, clear Output on match for timer 2 channel 0
    TPM2_C0SC &= (TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
    TPM2_C0SC |= (TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1));
    
    // Set Edge-aligned PWM, clear Output on match for timer 2 channel 1
    TPM2_C1SC &= (TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
    TPM2_C1SC |= (TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1));
}

/* Returns PWM value from duty cycle */
int getPWM(int duty_cycle){
	return ((float) duty_cycle / 100) * (7500+1);
}

/* Executes move based on direction settings */
void move(int dir) {
	if (dir == FORWARD) {
		TPM1_C0V = getPWM(FAST);   	// Right forward
		TPM1_C1V = 0;				
		TPM2_C0V = getPWM(FAST);	// Left forward
		TPM2_C1V = 0;
	} else if (dir == REVERSE) {
		TPM1_C0V = 0;				// Right reverse
		TPM1_C1V = getPWM(FAST);
		TPM2_C0V = 0;				// Left reverse
		TPM2_C1V = getPWM(FAST);
	} else if (dir == LEFT) {		// Left turn
		TPM1_C0V = getPWM(FAST);	// Right forward
		TPM1_C1V = 0;
		TPM2_C0V = 0;               // Left reverse
		TPM2_C1V = getPWM(SLOW);
	} else if (dir == RIGHT) {		// Right turn
		TPM1_C0V = 0;               // Right reverse
		TPM1_C1V = getPWM(SLOW);
		TPM2_C0V = getPWM(FAST);    // Left forward
		TPM2_C1V = 0;
	} else {						// STOP
		TPM1_C0V = 0;
		TPM1_C1V = 0;
		TPM2_C0V = 0;
		TPM2_C1V = 0;
	}
}
