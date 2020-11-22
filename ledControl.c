#include "MKL25Z4.h"                    // Device header
#include "ledControl.h"

/* Initializes LEDs */
void initLed(void) { 
	// Enable Clock to PORTC
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK); 

    // Configure MUX settings to make all LED pins GPIO
    PORTC->PCR[GREEN_LED_1] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[GREEN_LED_1] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[GREEN_LED_2] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[GREEN_LED_2] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[GREEN_LED_3] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[GREEN_LED_3] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[GREEN_LED_4] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[GREEN_LED_4] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[GREEN_LED_5] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[GREEN_LED_5] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[GREEN_LED_6] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[GREEN_LED_6] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[GREEN_LED_7] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[GREEN_LED_7] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[GREEN_LED_8] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[GREEN_LED_8] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[RED_LEDS] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[RED_LEDS] |= PORT_PCR_MUX(1);
    
    // Set Data Direction Registers for PortC
    PTC->PDDR |= (ALL_GREEN_LEDS_MASK | MASK(RED_LEDS));
}

/* Turns off all LEDs */
void offAllLeds(void) {
    PTC->PCOR = ALL_GREEN_LEDS_MASK;    // Off 8 green leds
    PTC->PCOR = MASK(RED_LEDS);         // Off 8 red leds
}

/* Turns on or off a specific LED */
void ledControl(uint32_t color, uint32_t status) {
    switch (color) {
        case GREEN_LED_1:
            if (status == LED_ON) {
                PTC->PSOR = MASK(GREEN_LED_1);
            } else {
                PTC->PCOR = MASK(GREEN_LED_1);
            }
            break;
        case GREEN_LED_2:
            if (status == LED_ON) {
                PTC->PSOR = MASK(GREEN_LED_2);
            } else {
                PTC->PCOR = MASK(GREEN_LED_2);
            }
            break;
        case GREEN_LED_3:
            if (status == LED_ON) {
                PTC->PSOR = MASK(GREEN_LED_3);
            } else {
                PTC->PCOR = MASK(GREEN_LED_3);
            }
            break;
        case GREEN_LED_4:
            if (status == LED_ON) {
                PTC->PSOR = MASK(GREEN_LED_4);
            } else {
                PTC->PCOR = MASK(GREEN_LED_4);
            }
            break;
        case GREEN_LED_5:
            if (status == LED_ON) {
                PTC->PSOR = MASK(GREEN_LED_5);
            } else {
                PTC->PCOR = MASK(GREEN_LED_5);
            }
            break;
        case GREEN_LED_6:
            if (status == LED_ON) {
                PTC->PSOR = MASK(GREEN_LED_6);
            } else {
                PTC->PCOR = MASK(GREEN_LED_6);
            }
            break;
        case GREEN_LED_7:
            if (status == LED_ON) {
                PTC->PSOR = MASK(GREEN_LED_7);
            } else {
                PTC->PCOR = MASK(GREEN_LED_7);
            }
            break;
        case GREEN_LED_8:
            if (status == LED_ON) {
                PTC->PSOR = MASK(GREEN_LED_8);
            } else {
                PTC->PCOR = MASK(GREEN_LED_8);
            }
            break;
        case RED_LEDS:
            if (status == LED_ON) {
                PTC->PSOR = MASK(RED_LEDS);
            } else {
                PTC->PCOR = MASK(RED_LEDS);
            }
            break;
        default:
            offAllLeds();
            break;
    }
}

/* Turns off all green LEDs */
void offAllGreenLeds(void) {
    PTC->PCOR = ALL_GREEN_LEDS_MASK;
}

/* Turns on 2 green LEDs when bluetooth is connected */
void on2GreenLeds(void) {
    offAllGreenLeds();
    PTC->PSOR = (MASK(GREEN_LED_4) | MASK(GREEN_LED_5));    
}

/* Turns on all green LEDs */
void onAllGreenLeds(void) {
    PTC->PSOR = ALL_GREEN_LEDS_MASK;
}
