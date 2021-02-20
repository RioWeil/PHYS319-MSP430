/*
 * PHYS319 Lab 3 Interrupt Example in C
 *
 * Written by Ryan Wicks
 * 16 Jan 2012
 *
 * This program is a C version of the assembly program that formed part of 
 * lab 2.
 *
 *
 */
#include  <msp430.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;      // Stop watchdog
  P1DIR = 0xF7;			//P1DIR set to 0xF7, or 11110111; P1.0-P1.2 and P1.4-P1.7 is output, P1.3 is input.
  P1OUT = 0x08;     //P1OUT set to 0x49, or 00001000; P1.3 set to high, everything else to low.
  P1REN = 0x08;     //enable pulldown resistor on P1.3
  P1IE = 0x08;		  //Enable input at P1.3 as an interrupt

  _BIS_SR (LPM4_bits + GIE);	//Turn on interrupts and go into the lowest
                               //power mode (the program stops here)
  	   	//Notice the strange format of the function, it is an "intrinsic"
  	   	//ie. not part of C; it is specific to this chipset
}

// Port 1 interrupt service routine
void __attribute__ ((interrupt(PORT1_VECTOR)))  PORT1_ISR(void)
{
  if (P1OUT == 0x08) {      // If P1OUT is 00001000 (Both off)
    P1OUT = 0x09;           // Sets P1OUT to 00001001 (Red on)
  }
  else if (P1OUT == 0x09) { // If P1OUT is 00001001 (Red on)
    P1OUT = 0x48;           // Sets P1OUT to 01001000 (Green on)
  }
  else if (P1OUT == 0x48) { // If P1OUT is 01001000 (Green on)
    P1OUT = 0x49;           // Sets P1OUT to 01001001 (Both on) 
  }
  else if (P1OUT == 0x49) { // If P1OUT is 01001001 (Both on)
    P1OUT = 0x08;           // Sets P1OUT to 00001000 (Both off)
  }
  P1IFG &= ~0x08;          // Clear P1.3 Interrupt flag. If you don't, it just happens again.
}