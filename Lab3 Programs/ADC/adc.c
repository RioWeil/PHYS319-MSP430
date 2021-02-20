//******************************************************************************
//  MSP430G2x31 Demo - ADC10, Sample A1, AVcc Ref, Set P1.0 if > 0.75*AVcc
//
//  Description: A single sample is made on A1 with reference to AVcc.
//  Software sets ADC10SC to start sample and conversion - ADC10SC
//  automatically cleared at EOC. ADC10 internal oscillator times sample (16x)
//  and conversion. 
//
//                MSP430G2x31
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|
//            |                 |           
//            |                 |
//            |                 |
//            |                 |
//            |                 |
// input  >---|P1.1/A1      P1.0|--> red Led onboard BIT0
//            |                 |
//            |             P1.2|--> yellow Led 
//            |             P1.6|--> green Led onboard BIT6
//
//
//  D. Dang
//  Texas Instruments Inc.
//******************************************************************************
#include "msp430.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                // Stop WDT
  ADC10CTL0 = ADC10SHT_2 + ADC10ON; 	     // ADC10ON
  ADC10CTL1 = INCH_1;                      // input A1
  ADC10AE0 |= 0x02;                        // PA.1 ADC option select
  P1DIR = 0x45 ;                           // Configure P1.0, P1.2, and P1.6 as output pins

  while (1)                                // Continually run the while loop.
  {
    ADC10CTL0 |= ENC + ADC10SC;            // Sampling and conversion start
    while (ADC10CTL1 &ADC10BUSY);          // ADC10BUSY?
    if (ADC10MEM < 0xF8) {                 // If input is below 0.8V
      P1OUT = 0x01;                        // Turn on red light by setting P1.0 high, rest low.
    }
    else if (ADC10MEM > 0x26D) {           // If input is above 2V
      P1OUT = 0x40;                        // Turn on green LED by setting P1.6 high, rest low.
    }
    else {                                 // Else (i.e. if input voltage in between 0.8V and 2V)
      P1OUT = 0x04;                        // Set P1.2 to high, which lights up blue LED on breadboard.
    }

    unsigned i;                             // Initialize counter variable
    for (i = 0xFFFF; i > 0; i--);           // Delay
  }
}
//******************************************************************************