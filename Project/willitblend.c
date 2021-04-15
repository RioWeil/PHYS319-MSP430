/*
Name: willitblend.c
Author: Rio Weil
Course: PHYS 319
Functionality: Makes MSP430 play the "Will it Blend?" theme through an external speaker
Pin setup:
P1.2: Connected to external speaker
*/

#include "msp430.h"

// Get period of waveform from frequency of note
int getperiod(int frequency) {
	int period = 1000000/frequency;
	return period;
}

// Waits by decrementing counter 5000 times
void wait() {
	for(volatile int i = 5000; i > 0; i--) {
	}
}

// Plays given note, for a period of waits times
void playnote(int note, int waits) {
	TACTL = TASSEL_2 + MC_0;        // SMCLK, down mode
	int period = getperiod(note);   // Convert note to period
	CCR0 = period;                  // PWM Period, in us.
	CCTL1 = OUTMOD_7;               // CCR1 reset/set
	CCR1 = period/2;                // CCR1 PWM duty cycle (halfperiod to get a clean tone)
	TACTL = TASSEL_2 + MC_1;        // SMCLK, up mode
	while(waits > 0) {              // Wait (play) for specified amount of 16th notes
		wait();
		waits--;
	}
}

// Rests for a period of wait times
void rest(int waits) {
	TACTL = TASSEL_2 + MC_0;        // SMCLK, down mode
	CCTL1 = OUTMOD_7;               // CCR1 reset/set
	CCR1 = 0;                       // CCR1 PWM duty cycle (set to zero so it plays nothing)
	TACTL = TASSEL_2 + MC_1;        // SMCLK, up mode
	while(waits > 0) {              // Wait (play) for specified amount of 16th notes
		wait();
		waits--;
	}
}

// Short break
void breathe() {
	TACTL = TASSEL_2 + MC_0;        // SMCLK, down mode
	CCTL1 = OUTMOD_7;               // CCR1 reset/set
	CCR1 = 0;                       // CCR1 PWM duty cycle (set to zero so it plays nothing)
	TACTL = TASSEL_2 + MC_1;        // SMCLK, up mode
	for(volatile int i = 3000; i > 0; i--) {
	}
}



void main(void) {
	// Definitions of different note frequencies
	int C4 = 262*0.9;
	int C4s = 277*0.9;
	int D4 = 294*0.9;
	int D4s = 311*0.9;
	int E4 = 330*0.9;
	int F4 = 349*0.9;
	int F4s = 379*0.9;
	int G4 = 392*0.9;
	int G4s = 415*0.9;
	int A4 = 440*0.9;
	int A4s = 466*0.9;
	int B4 = 494*0.9;
	int C5 = 523*0.9;
	int D5 = 587*0.9;
	int E5 = 659*0.9;

	// Initializations
	WDTCTL = WDTPW + WDTHOLD;       // Stop WDT
	P1DIR |= BIT2;                  // P1.2 to output
	P1SEL |= BIT2;                  // P1.2 to TA0.1

	// Song starts
	playnote(D4, 4);
	breathe();
	playnote(D4, 4);
	playnote(G4, 4);
	playnote(E5, 3);
	rest(3);
	playnote(D5, 24);
	breathe();
	playnote(C5, 8);
	playnote(D5, 4);
	playnote(E5, 3);
	rest(3);
	playnote(D5, 4);
	playnote(C5, 4);
	playnote(D5, 24);
	rest(4);
	playnote(D4, 4);
	breathe();
	playnote(D4, 4);
	playnote(G4, 4);
	playnote(E5, 3);
	rest(3);
	playnote(D5, 24);
	breathe();
	playnote(C5, 8);
	playnote(D5, 4);
	playnote(E5, 3);
	rest(3);
	playnote(D5, 4);
	playnote(C5, 4);
	playnote(D5, 24);
	rest(1);
}

