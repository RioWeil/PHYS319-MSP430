#include "msp430.h"

// Get period of waveform from freuency of note
int getperiod(int frequency) {
	int period = 1000000/frequency;
	return period;
}

// Waits by decrementing counter 10000 times (one quarter note)
void wait() {
	for(volatile int i = 12500; i > 0; i--) {
	}
}

// Plays given note, for a period of waits times (waits = 1 is a 16th note)
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

// Rests for a period of wait times (waits = 1 is a 16th rest)
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


	WDTCTL = WDTPW + WDTHOLD;       // Stop WDT
	P1DIR |= BIT2;                  // P1.2 to output
	P1SEL |= BIT2;                  // P1.2 to TA0.1
	rest(2);
	playnote(G4, 2);
	playnote(A4, 2);
	playnote(G4, 2);
	playnote(A4, 8);
	rest(2);
	playnote(G4, 2);
	playnote(A4, 2);
	playnote(G4, 2);
	playnote(A4, 4);
	playnote(C5, 2);
	playnote(G4, 9);
	rest(8);
	rest(2);
	playnote(F4, 2);
	playnote(G4, 2);
	playnote(F4, 2);
	playnote(G4, 2);
	playnote(C5, 4);
	playnote(F4, 10);
	rest(13);
	playnote(D4, 1);
	playnote(F4, 1);
	playnote(D4, 1);
	playnote(F4, 3);
	breathe();
	playnote(F4, 1);
	playnote(D4, 1);
	playnote(C4, 3);
	// Verse part 1 over
	rest(2);
	playnote(G4, 2);
	playnote(A4, 2);
	playnote(G4, 2);
	playnote(A4, 8);
	rest(2);
	playnote(G4, 2);
	playnote(A4, 2);
	playnote(G4, 2);
	playnote(A4, 2);
	playnote(C5, 6);
	playnote(G4, 8);
	rest(8);
	rest(2);
	playnote(F4, 2);
	playnote(G4, 2);
	playnote(F4, 2);
	playnote(G4, 2);
	playnote(C5, 4);
	playnote(F4, 10);
	rest(12);
	playnote(D4, 1);
	playnote(F4, 1);
	playnote(D4, 1);
	playnote(F4, 3);
	breathe();
	playnote(F4, 1);
	playnote(D4, 1);
	playnote(C4, 3);
	rest(2);
	playnote(C4, 8);
	playnote(F4, 8);
	// Verse part 2 over
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4s, 2);
	breathe();
	playnote(A4s, 2);
	breathe();
	playnote(A4s, 2);
	breathe();
	playnote(A4s, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	playnote(A4, 2);
	breathe();
	// Bridge over
	playnote(D5, 4);
	playnote(F4, 4);
	playnote(D5, 6);
	playnote(C5, 2);
	rest(2);
	playnote(F4, 2);
	breathe();
	playnote(F4, 2);
	breathe();
	playnote(C5, 2);
	breathe();
	playnote(C5, 2);
	playnote(A4, 2);
	breathe();
	playnote(A4, 4);
	// Chorus part 1 over
	rest(4);
	playnote(F4, 4);
	playnote(D5, 6);
	playnote(C5, 2);
	rest(2);
	playnote(F4, 2);
	breathe();
	playnote(F4, 2);
	breathe();
	playnote(C5, 2);
	breathe();
	playnote(C5, 2);
	playnote(A4, 2);
	breathe();
	playnote(A4, 4);
	// Chorus part 2 over
	rest(4);
	playnote(F4, 4);
	playnote(D5, 6);
	playnote(C5, 2);
	rest(2);
	playnote(F4, 2);
	breathe();
	playnote(F4, 2);
	breathe();
	playnote(C5, 2);
	breathe();
	playnote(C5, 2);
	playnote(A4, 2);
	breathe();
	playnote(A4, 4);
	playnote(D4, 16);
	rest(2);
	playnote(F4, 2);
	breathe();
	playnote(F4, 2);
	breathe();
	playnote(C5, 2);
	breathe();
	playnote(C5, 2);
	playnote(A4, 2);
	breathe();
	playnote(A4, 4);
	rest(1);
	// Song over	

}

