#include "MKL25Z4.h"                    // Device header
#include "motors.h"

void initMotors(void) {

	// Enable Clock for PORTD
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	//  Timer 0 CH0
	PORTD->PCR[RIGHT_MOTOR_FORWARD] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[RIGHT_MOTOR_FORWARD] |= PORT_PCR_MUX(4);
	
	// Timer 0 CH3
	PORTD->PCR[LEFT_MOTOR_FORWARD] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LEFT_MOTOR_FORWARD] |= PORT_PCR_MUX(4);

	// Timer 0 CH5
	PORTD->PCR[RIGHT_MOTOR_REVERSE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[RIGHT_MOTOR_REVERSE] |= PORT_PCR_MUX(4);

	// Timer 0 CH2
	PORTD->PCR[LEFT_MOTOR_REVERSE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LEFT_MOTOR_REVERSE] |= PORT_PCR_MUX(4);
	
	// Enable Clock gating for Timer 0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	// Select Clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// Modulo value 255
	TPM0->MOD = 0xff;
	
	// Enable PWM on TPM1 Channel 0 -> PTB0
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(1));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	// Enable PWM on TPM channel 0 -> PTD0 and channel 3 -> PTD3
	TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Enable PWM on TPM channel 2 -> PTD2 and channel 5 -> PTD5
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C5SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

int PWM(int duty_cycle){
	return ((float) duty_cycle / 100) * (0xff+1);
}

void move(int dir) {
	if (dir == FORWARD) {
		TPM0_C0V = PWM(FAST);				// RIGHT FORWARD
		TPM0_C3V = PWM(FAST);				// LEFT FORWARD
	} else if (dir == REVERSE) {
		TPM0_C5V = PWM(FAST);				// LEFT REVERSE
		TPM0_C2V = PWM(FAST);				// RIGHT REVERSE
	} else if (dir == LEFT) {
		TPM0_C0V = PWM(FAST);				// RIGHT FORWARD
		TPM0_C5V = PWM(FAST);				// LEFT	REVERSE
	} else if (dir == RIGHT) {
		TPM0_C2V = PWM(FAST);				// RIGHT REVERSE	
		TPM0_C3V = PWM(FAST);				// LEFT FORWARD
	} else if (dir == F_LEFT) {
		TPM0_C0V = PWM(FAST);				// RIGHT FORWARD
		TPM0_C3V = PWM(SLOW);				// LEFT FORWARD
	} else if (dir == F_RIGHT) {
		TPM0_C0V = PWM(SLOW);				// RIGHT FORWARD
		TPM0_C3V = PWM(FAST);				// LEFT FORWARD
	} else if (dir == R_LEFT) {
		TPM0_C5V = PWM(SLOW);				// LEFT REVERSE
		TPM0_C2V = PWM(FAST);				// RIGHT REVERSE
	} else if (dir == R_RIGHT) {
		TPM0_C5V = PWM(FAST);				// LEFT REVERSE
		TPM0_C2V = PWM(SLOW);				// RIGHT REVERSE
	} else {								// STOP
		TPM0_C0V = 0;
		TPM0_C5V = 0;
		TPM0_C2V = 0;
		TPM0_C3V = 0;
	}
}