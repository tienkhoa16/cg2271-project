#include "MKL25Z4.h"                    // Device header
#include "uart.h"

/* Init UART2 */
void initUART2(void) {
    uint32_t divisor, bus_clock;
    
    // Enable clock for UART2 and Port E
    SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    // Enable UART2 receiver on Port 2 pin 23
    PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
    
    // Disable UART2 transmitter and receiver
    UART2_C2 &= ~(UART_C2_RE_MASK);
    
    // Get default bus clock
    bus_clock = DEFAULT_SYSTEM_CLOCK / 2;
    
    // Get division factor
    divisor = bus_clock / (BAUD_RATE * 16);
    
    UART2->BDH = UART_BDH_SBR(divisor >> 8);
    UART2->BDL = UART_BDL_SBR(divisor);
    
    UART2_C1 = 0;
    UART2_S2 = 0;
    UART2_C3 = 0;
    
    NVIC_SetPriority(UART2_IRQn, UART2_INT_PRIO);
    NVIC_ClearPendingIRQ(UART2_IRQn);
    NVIC_EnableIRQ(UART2_IRQn);
    
    UART2_C2 |= (UART_C2_RE_MASK);
    UART2_C2 |= (UART_C2_RIE_MASK);
}
