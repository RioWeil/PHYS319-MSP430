/*
 * main.c
 *
 * MSP-EXP430G2-LaunchPad User Experience Application
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *

 * Heavily modified: Nov, 2013 Carl Michal
 * Even more so, February, 2014
 * Further heavily modified: March, 2021 Rio Weil (with help from Luna Liu)

 * This version modified to use the Hardware UART on MSP430G2553
 * see code at https://bennthomsen.wordpress.com/engineering-toolbox/ti-msp430-launchpad/msp430g2553-hardware-uart/
 * jumpers for TX/RX on Launchpad board must be rotated 90 degrees
 * for hardware UART used here!!
 * This may not work on all revisions of the board?


*/


/******************************************************************************
 *                  MSP-EXP430G2-LaunchPad User Experience Application
 *
 * 1. Device starts up in LPM3 + blinking LED to indicate device is alive    
 *    + Upon first button press, device transitions to application mode
 * 2. Application Mode
 *    + Continuously sample ADC Temp Sensor channel
 *       
 *    + Transmit temperature value via TimerA UART to PC  
 * 
 *
 * Texas Instruments, Inc.
 ******************************************************************************/
  
#include  "msp430.h"

#define     TRIG                  BIT4                      // P1.4 connected to trigger pin
#define     ECHO                  BIT5                      // P1.5 connected to Echo pin

#define     TXD                   BIT2                      // TXD on P1.2
#define     RXD                   BIT1                      // RXD on P1.1

unsigned int TXByte;
volatile unsigned long timeMeasured;

void main(void)
{

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  /* next three lines to use internal calibrated 1MHz clock: */
  BCSCTL1 = CALBC1_1MHZ;                    // Set range
  DCOCTL = CALDCO_1MHZ;
  BCSCTL2 &= ~(DIVS_3);                     // SMCLK = DCO = 1MHz  


  /* Reset clock and set to continuous mode */
  TACTL = TACLR;          // Reset timer
  TACTL=TASSEL_2|MC_2;    // Continous mode


  /* Setup output pins */
  P1DIR |= TXD + TRIG;
  P1OUT |= TXD;

  /* Configure hardware UART */
  P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2; // Use SMCLK
  UCA0BR0 = 104; // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
  UCA0BR1 = 0; // Set baud rate to 9600 with 1MHz clock
  UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine

  
  /* Main Application Loop */
  while(1)
  {    

    /* Sends out the hard Trigger pulse to the Trigger pin on the sensor */
    P1OUT |= TRIG;        // Set P1.4 to high, setting the trigger pin to high
    __delay_cycles(10);   // Wait 10 us
    P1OUT &= ~TRIG;          // Stop the high trigger signal

    /* Waits until echo pin is high, and then starts counting */
    while (1) {           // Waits until P1.5 reads high (ECHO pin is outputting high)
      if ((P1IN | 0xDF) == 0xFF) {  // Checks if P1.5 is high/ECHO is high (measurement start)
        TAR = 0;          // Resets Timer to zero
        break;            // breaks out of loop
      }
    }

    /* Waits until echo pin switches to low, and then stops counting */
    while (1) {          // Waits until P1.5 reads low (ECHO pin is outputting low)
      if ((P1IN & 0x20) == 0x0) {  // Checks if P1.5 is low/ECHO is low (pulse has come back)
        timeMeasured = TAR; // Sets timeMeasured to the Time-of-flight of the pulse (how long ECHO was high for)
        break;
      }
    }
    
    /* Send data to computer */
    TXByte = (unsigned char)(timeMeasured / 58); // Divide by 58 to get the distance to object (cm) from the TOF
    while (! (IFG2 & UCA0TXIFG)); // Wait for TX buffer to be ready for new data
    UCA0TXBUF = TXByte;           // Write new data

    __delay_cycles(20000);  // Waits 20 ms before sending next trigger pulse.
  }  
}

