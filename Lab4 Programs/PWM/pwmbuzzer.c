#include "msp430.h"
void main(void) {
	int frequency = 600;             // Input frequency
	frequency = frequency*0.9;
	int period = 1000000/frequency;  // Convert frequency into corresponding period in us
	int halfperiod = period/2;       // Halve the period for CCR1 value


	WDTCTL = WDTPW + WDTHOLD;       // Stop WDT
	P1DIR |= BIT2;                  // P1.2 to output
	P1SEL |= BIT2;                  // P1.2 to TA0.1
	CCR0 = period;                  // PWM Period, in us.
	CCTL1 = OUTMOD_7;               // CCR1 reset/set
	CCR1 = halfperiod;              // CCR1 PWM duty cycle
	TACTL = TASSEL_2 + MC_1;        // SMCLK, up mode
	_BIS_SR(LPM0_bits);             // Enter Low Power Mode 0
}

