#include "msp430.h"
void main(void) {
	int period = 1023;                        // Period of PWM in us

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  	ADC10CTL0 = ADC10SHT_2 + ADC10ON; 	      // ADC10ON
  	ADC10CTL1 = INCH_1;                       // input A1
  	ADC10AE0 |= 0x02;                         // PA.1 ADC option select
	P1DIR |= BIT2;                            // P1.2 to output
	P1SEL |= BIT2;                            // P1.2 to TA0.1
	CCR0 = period;                            // PWM Period

	while (1) {
		ADC10CTL0 |= ENC + ADC10SC;           // Sample P1.1 (ADC)
		while (ADC10CTL1 &ADC10BUSY) {
				CCTL1 = OUTMOD_7;             // CCR1 reset/set
				CCR1 = ADC10MEM;              // Set PWM pulsewidth to be the value from the ADC 
				TACTL = TASSEL_2 + MC_1;      // SMCLK, up mode
		}
	}
}



