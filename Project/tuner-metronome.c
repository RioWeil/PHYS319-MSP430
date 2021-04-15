/*
Name: project.c
Author: Rio Weil
Course: PHYS 319
Functionality: Makes MSP430 to run a metronome (on run) or tuner (on pressing P1.3 interrupt)
Pin setup:
P1.0: No Connection
P1.1: Connected to op-amp/comparator processed audio input from electret mic
P1.2: Connected to external speaker
P1.3: No Connection 
P1.4: Connected to button that grounds the pin when pressed
P1.5: Connected to button that grounds the pin when pressed
P1.6: Connected to RS pin on LCD Display
P1.7: Connected to Enable pin on LCD Display
P2.0 - P2.5, P2.6 (XOUT), P2.7 (PIN): Connected to DB0-DB7 pins on LCD Display
*/

#include  <msp430.h>
#include <stdio.h>   // Importing stdio library for int to string conversion

#define RS_HIGH P1OUT = P1OUT | BIT6      // Set RS pin high (for writing characters to display)
#define RS_LOW P1OUT = P1OUT & (~BIT6)    // Set RS pin low (for writing commands to display)
#define E_HIGH P1OUT = P1OUT | BIT7       // Set Enable high signal
#define E_LOW P1OUT = P1OUT & (~BIT7)     // Set Enable low signal (falling edge trigger)
// Interfacing stuff inspired by https://www.instructables.com/Interfacing-16x2-LCD-with-msp430-launchpad-in-8-bi/

// Waits 1ms
void wait_1ms() {
	__delay_cycles(1000);
}

// Writes data (command or char) to LCD display by making the E pin go from high and then back to low.
void data_write(void) {
	E_HIGH;
	__delay_cycles(2);
	E_LOW;
}

// Write command to LCD display
void command(char i) {
	__delay_cycles(1600);
	P2OUT = i;
	RS_LOW;
	data_write();
}

// Write character to LCD display
void write(char i) {
	__delay_cycles(1600);
	P2OUT = i;
	RS_HIGH;
	data_write();
}

// Write a string to LCD display
void write_string(char *s) {
	while (*s) {   // Loop over characters in string, write one at a time
		write(*s);
		s++;
	}
}

// Initialization sequence for the LCD display
void initdisplay() {
	E_LOW;
	__delay_cycles(100); // Wait after power is applied
	command(0x30);       // Wake up #1
	__delay_cycles(30);  // Wait
	command(0x30);       // Wake up #2
	__delay_cycles(10);  // Wait
	command(0x30);       // Wake up #3
	__delay_cycles(10);  // Wait
	command(0x38);       // Function set: 8-bit/2-line
	command(0x01);       // Clear screen
	command(0x10);       // Set cursor
	command(0x0F);       // Display ON, Cursor ON
	command(0x06);       // Entry mode set
}

// Command that clears the numbers
void clearnums() {
	command(0x10);
	command(0x10);
	command(0x10);       // LCD cursor goes back three places
	write_string("   "); // Write 3 blank characters
	command(0x10);
	command(0x10);
	command(0x10);       // LCD cursor goes back three places
}

// Update the LCD display with the new beats per minute value
void updatedisplaybpm(int bpm) {
	clearnums();                 // Clear current bpm on LCD display

	if (bpm < 10) {              // Case if bpm is 1 char
		char str[1];             // New 1 character array
		sprintf(str, "%d", bpm); // Convert int bpm to the string
		write_string(str);       // Write the converted string to the display
		command(0x14);
		command(0x14);           // Move the cursor up two spaces
	} else if (bpm < 100) {
		char str[2];
		sprintf(str, "%d", bpm);
		write_string(str);
		command(0x14);
	} else {
		char str[3];
		sprintf(str, "%d", bpm);
		write_string(str);
	}
	for(volatile int i = 100; i > 0; i--) { // Wait 100ms after updating display to give hardware time to catch up
		wait_1ms();
	}

}

// Convert bpm to the PWM output wave period.
int periodfrombpm(int bpm) {
	return 60000 / bpm; // 60ms divided by the bpm (gives period in units of 1000ms)
}

// Main function for the metronome mode
void playmet() {
	volatile unsigned int bpm = 100;  // Keeping track of current bpm
	volatile unsigned char check1;    // Keeping track of whether the decrement bpm button is pressed
	volatile unsigned char check2;    // Keeping track of whether the increment bpm button is pressed
	volatile unsigned int numrests;   // Integer to keep track of number of rests for waiting between beeps
	volatile int waits;               // Counter variable

	write_string("Metronome Mode      BPM:");
	write_string("100");                       // Initial lines to write on LCD

	while(1) {                                 // Main function loop

		// Playing beep
		TACTL = TASSEL_2 + MC_0;               // SMCLK, down mode
		CCR0 = 2400;                           // PWM Period, in us.
		CCTL1 = OUTMOD_7;                      // CCR1 reset/set
		CCR1 = 1200;                           // CCR1 PWM duty cycle (halfperiod to get a clean tone)
		TACTL = TASSEL_2 + MC_1;               // SMCLK, up mode
		for(waits = 0; waits < 100; waits++) { // Play beep for 100ms
			wait_1ms();
			check1 = P1IN & BIT4;              // 0 if P1.4 is low (button pressed), 00010000 otherwise (pulled high by default)
			check2 = P1IN & BIT5;              // 0 if P1.5 is low (button pressed), 00100000 otherwise (pulled high by default)
			if (check2 == 0) {                 // If increment button pressed  
		    	if (bpm < 250) {               // 250 is upper limit on bpm
		    		bpm++;
		    		updatedisplaybpm(bpm);
		    	}
			} else if (check1 == 0) {          // If decrement button pressed
				if (bpm > 20) {                // 20 is lower limit on bpm
					bpm--;
					updatedisplaybpm(bpm);
				}
			}
		}

		// Waiting in between beeps
		TACTL = TASSEL_2 + MC_0;
		CCTL1 = OUTMOD_7;
		CCR1 = 0;                                    // CCR1 PWM duty cycle (set to zero so it plays nothing)
		TACTL = TASSEL_2 + MC_1;
		numrests = periodfrombpm(bpm) - 100;          // Get number of ms for period minus 100 (the note playing period)
		for(waits = 0; waits < numrests; waits++) {  // Wait for specified amount of time
			wait_1ms();
			check1 = P1IN & BIT4;
			check2 = P1IN & BIT5;
			if (check2 == 0) {   
		    	if (bpm < 250) {
		    		bpm++;
		    		updatedisplaybpm(bpm);
		    	}
			} else if (check1 == 0) {
				if (bpm > 20) {
					bpm--;
					updatedisplaybpm(bpm);
				}
			}
		}
		
	}
}

// Lookup function for getting the string of a note from the integer key
char* int_to_note_string(int note) {
	switch(note) {
		case 16:
		return "C2";

		case 17:
		return "C#2";

		case 18:
		return "D2";

		case 19:
		return "D#2";

		case 20:
		return "E2";

		case 21:
		return "F2";

		case 22:
		return "F#2";

		case 23:
		return "G2";

		case 24:
		return "G#2";

		case 25:
		return "A2";

		case 26:
		return "A#2";

		case 27:
		return "B2";

		case 28:
		return "C3";

		case 29:
		return "C#3";

		case 30:
		return "D3";

		case 31:
		return "D#3";

		case 32:
		return "E3";

		case 33:
		return "F3";

		case 34:
		return "F#3";

		case 35:
		return "G3";

		case 36:
		return "G#3";

		case 37:
		return "A3";

		case 38:
		return "A#3";

		case 39:
		return "B3";

		case 40:
		return "C4";

		case 41:
		return "C#4";

		case 42:
		return "D4";

		case 43:
		return "D#4";

		case 44:
		return "E4";

		case 45:
		return "F4";

		case 46:
		return "F#4";

		case 47:
		return "G4";
		
		case 48:
		return "G#4";

		case 49:
		return "A4";

		case 50:
		return "A#4";

		case 51:
		return "B4";

		case 52:
		return "C5";

		case 53:
		return "C#5";
		
		case 54:
		return "D5";

		case 55:
		return "D#5";

		case 56:
		return "E5";

		case 57:
		return "F5";

		case 58:
		return "F#5";

		case 59:
		return "G5";

		case 60:
		return "G#5";

		case 61:
		return "A5";

		case 62:
		return "A#5";

		case 63:
		return "B5";

		case 64:
		return "C6";

		case 65:
		return "C#6";

		case 66:
		return "D6";

		case 67:
		return "D#6";

		case 68:
		return "E6";

		case 69:
		return "F6";

		case 70:
		return "F#6";

		case 71:
		return "G6";
		
		case 72:
		return "G#6";

		case 73:
		return "A6";

		case 74:
		return "A#6";

		case 75:
		return "B6";

		case 76:
		return "C7";

		default:
		return "Error";
	}
}

// Lookup function to get the frequency of the note from the integer key.
int int_to_note_freq(int note) {
	switch (note) {
		case 16:
		return 65.40;

		case 17:
		return 69.29;

		case 18:
		return 73.4;

		case 19:
		return 77.78;

		case 20:
		return 82.41;

		case 21:
		return 87.31;

		case 22:
		return 92.49;

		case 23:
		return 98;

		case 24:
		return 103.82;

		case 25:
		return 110;

		case 26:
		return 116.54;

		case 27:
		return 123.47;

		case 28:
		return 130.81;

		case 29:
		return 138.59;

		case 30:
		return 146.83;

		case 31:
		return 155.56;

		case 32:
		return 164.81;

		case 33:
		return 174.61;

		case 34:
		return 185;

		case 35:
		return 196;

		case 36:
		return 207.65;

		case 37:
		return 220;

		case 38:
		return 233.08;

		case 39:
		return 246.94;

		case 40:
		return 261.63;

		case 41:
		return 277.18;

		case 42:
		return 293.66;

		case 43:
		return 311.13;

		case 44:
		return 329.63;

		case 45:
		return 349.23;

		case 46:
		return 369.99;

		case 47:
		return 392;

		case 48:
		return 415.30;

		case 49:
		return 440;

		case 50:
		return 466.16;

		case 51:
		return 493.88;

		case 52:
		return 523.25;

		case 53:
		return 554.37;

		case 54:
		return 587.33;

		case 55:
		return 622.25;

		case 56:
		return 659.25;

		case 57:
		return 698.46;

		case 58:
		return 739.99;

		case 59:
		return 783.99;

		case 60:
		return 830.61;

		case 61:
		return 880;

		case 62:
		return 932.33;

		case 63:
		return 987.77;

		case 64:
		return 1047.5;

		case 65:
		return 1108.73;

		case 66:
		return 1174.65;

		case 67:
		return 1244.51;

		case 68:
		return 1318.51;

		case 69:
		return 1396.91;

		case 70:
		return 1479.98;

		case 71:
		return 1567.98;

		case 72:
		return 1661.22;

		case 73:
		return 1760;

		case 74:
		return 1864.66;

		case 75:
		return 1975.53;

		case 76:
		return 2093;

		default:
		return 440;
	}
}

// Update the PWM output with the new note/frequency to be played.
void updateplaynote(int note) {
	volatile int period = 1000000/int_to_note_freq(note); // Get period by dividing frequency by 1E6 microseconds
	TACTL = TASSEL_2 + MC_0;                              // SMCLK, down mode
	CCR0 = period;                                        // PWM Period, in us.
	CCTL1 = OUTMOD_7;                                     // CCR1 reset/set
	CCR1 = period/2;                                      // CCR1 PWM duty cycle (halfperiod to get a clean tone)
	TACTL = TASSEL_2 + MC_1;                              // SMCLK, up mode
	for(volatile int i = 100; i > 0; i--) {
		wait_1ms();
	}
}

// Taken from https://www.programmingsimplified.com/c-program-find-string-length
// Return length of string
int string_length(char *s) {
  if (*s == '\0') // Base condition
    return 0;

  return (1 + string_length(++s));
}

// Update the LCD display with the current note being tuned
void updatedisplaynote(int note) {
	command(0x01);                            // Clear screen
	command(0x10);                            // Set cursor
	command(0x0F);                            // Display ON, Cursor ON
	command(0x06);                            // Entry mode set
	write_string("Tuning Mode Note:");
	char* notestr = int_to_note_string(note); // Get note title from the integer key
 	write_string(notestr);                    // Write to LCD display
 	if (string_length(notestr) == 2) {        // If note is just 2 chars long, write an extra space
 		write_string(" ");
 	}
 	for(volatile int i = 100; i > 0; i--) {   // Wait 100ms
		wait_1ms();
	}

}

// Update the LCD display with the differnece between the target note/frequency and played frequency
void updatedisplaydifference(int difference) {
	char str[12];                            // Large enough to hold any possible difference
	sprintf(str, "%d", difference);          // Convert frequency difference into string
	if (difference > 0) {                    // If difference is positive, write a + char (- char gets written automatically if diff. is negative)
		write_string("+");
	}
	write_string(str);                       // Write to display
	for(volatile int i = 100; i > 0; i--) {
		wait_1ms();
	}
}



void tunenote(int note) {
	volatile int pulsewidth;
	volatile int difference;
	volatile unsigned char checkplay;
	TAR = 0;
	while(1) {
		checkplay = P1IN & BIT1;
		if (checkplay == BIT1) {
			break;
		}
	}
	while(1) {
		checkplay = P1IN & BIT1;
		if (checkplay == 0) {
			break;
		}
	}
	pulsewidth = TAR;
	difference = 1000000/pulsewidth - int_to_note_freq(note);
	updatedisplaynote(note);
	updatedisplaydifference(difference);
}

// Main function for the tuning mode
void tuner() {
	volatile unsigned int note = 40;
	volatile unsigned char check1;
	volatile unsigned char check2;
	volatile unsigned char checkplay;

	updateplaynote(note);
	updatedisplaynote(note);

	while(1) {
		check1 = P1IN & BIT4;
		check2 = P1IN & BIT5;
		checkplay = P1IN & BIT1;
		if (check2 == 0) {   
	    	if (note < 76) {
	    		note++;
	    		updateplaynote(note);
	    		updatedisplaynote(note);
	    	}
		} else if (check1 == 0) {
			if (note > 16) {
	    		note--;
	    		updateplaynote(note);
	    		updatedisplaynote(note);
			}
		} else if (checkplay == 0) {
			tunenote(note);
		}
	}

}


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;      // Stop watchdog
	
	P2SEL &= ~BIT6;
	P2SEL &= ~BIT7;                 // Free up XIN and XOUT pins to use for display
	P1DIR = 0xCF;                   // P1 pins are all output except for P1.4 and P1.5 for reading in buttons
	P2DIR = 0xFF;                   // P2 pins are all output (display)

	P1REN = BIT4 | BIT5;
	P1OUT = BIT4 | BIT5;            // Enable pullup resistors on P1.4/P1.5 to keep them at high
	
	P2OUT = 0x00;

	P1SEL |= BIT2;                  // P1.2 to TA0.1

	P1DIR &= (~BIT3);               // P1.3 to input
	P1OUT |= BIT3;
	P1REN |= BIT3;                  // Enable pullup resistor on P1.3
	P1IE |= BIT3;                   // Enable interrupt on P1.3
	_BIS_SR (GIE);                  // Set global interrupt flag
	P1IFG &= (~BIT3);               // Clear the interrupt flag on P1.3 (in case running program again, so it doesn't immediately jump)

	initdisplay();                  // Initialize LCD display
	playmet();                      // Go into the metronome mode
}

// Port 1 interrupt service routine (interrupt when P1.3 button is pressed)
void __attribute__ ((interrupt(PORT1_VECTOR)))  PORT1_ISR(void)
{ 					
  P1IFG &= (~BIT3);      // Clear P1.3 Interrupt flag.
  P1DIR &= (~BIT1);      // Set P1.1 as input
  P1REN |= BIT1;             
  P1OUT |= BIT1;         // Enable pullup resistor on P1.1

  // Setup internal calibrated 1MHz clock
  BCSCTL1 = CALBC1_1MHZ; // Set range
  DCOCTL = CALDCO_1MHZ;
  BCSCTL2 &= ~(DIVS_3);  // SMCLK = DCO = 1MHz  

  TACTL = TACLR;         // Reset timer
  TACTL=TASSEL_2|MC_2;   // Continous mode

  initdisplay();         // Initialize display
  tuner();               // Go into the tuner mode
}