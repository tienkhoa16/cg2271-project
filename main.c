#include "MKL25Z4.h"                    // Device header


#define PTB0_PIN 0                      // Port B pin 0, TPM1_CH0
#define PTB1_PIN 1                      // Port B pin 1, TPM1_CH1
#define PTB2_PIN 2                      // Port B pin 2, TPM2_CH0
#define PTB3_PIN 3                      // Port B pin 3, TPM2_CH1

/* init PWM */
void initPwm(void) {
    // Enable clock gating port B
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
    
    // Enable timer module 1 channel 0
    PORTB->PCR[PTB0_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB0_PIN] |= PORT_PCR_MUX(3);
    
    // Enable timer module 1 channel 1
    PORTB->PCR[PTB1_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB1_PIN] |= PORT_PCR_MUX(3);

    // Enable timer module 2 channel 0
    PORTB->PCR[PTB2_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB2_PIN] |= PORT_PCR_MUX(3);
    
    // Enable timer module 2 channel 1
    PORTB->PCR[PTB3_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB3_PIN] |= PORT_PCR_MUX(3);
    
    // Enable clock for timer module 1 and timer module 2
    SIM_SCGC6 |= (SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK);
    
    // Choose source clock
    SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
    
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

static void delay(volatile uint32_t nof) {
	while(nof!=0) {
		__asm("NOP");
		nof--;
	}
}

int main(void) {
    SystemCoreClockUpdate();
    initPwm();
    
    TPM1_C0V = 3750;    // going forward
    TPM1_C1V = 0;       // going backward
    TPM2_C0V = 3750;
    TPM2_C1V = 3750;
    
    while (1) {

    }
    
}
