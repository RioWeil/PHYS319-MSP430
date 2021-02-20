/*
 * PHYS319 Lab3 Timing example in C
 *
 * Written by Ryan Wicks
 * 16 January 2012
 *
 * This program is a C version of the assembly program that formed part of lab 2.
 * This is not the best way to implement timing, or to organize your code. 
 * It is simply one way.
 *
 * This will almost certainly not give exactly the same timing as the assembly
 * program from lab 2, and the output assembly will also be very different, even
 * though the task is similar.
 */

#include <msp430.h> // Imports functionality for programming the msp430

void main(void) {
	volatile unsigned int count; // Declare integer; voltatile tells compiler that variable can change unexpectedly
	                             // (e.g. interrupt) hence, we should store the variable in RAM and not just the register.
	WDTCTL = WDTPW + WDTHOLD;    // Stop the watchdog.
	P1DIR = 0x41;			// Set P1 output direction (e.g. set P1.0 and P1.6 to outputs)
	P1OUT = 0x01;			// Sets P1.0 to high.
	
	while (1){				// Loop forever (1 is true in C, while loop runs while condition is true)
		count = 60000;      // Counter variable.
		while(count != 0) { // Decrements while counter is not zero (i.e. positive)
			count--;		// Decrement the count
		}
		count = 60000;      // Do the decrementing while loop again to make frequnecy of switching twice as low.
		while(count != 0) {
			count--;
		}
		P1OUT = P1OUT ^ 0x41;	// Bitwise xor the output with 0x41 (flips which LED is on)
	}
}