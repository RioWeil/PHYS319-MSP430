#include "msp430g2553.inc"
	org 0x0C000
RESET:
	mov.w #0x400, sp               ; Initialize the stack pointer
	mov.w #WDTPW|WDTHOLD,&WDTCTL   ; Disables the watchdog
	mov.b #11110111b, &P1DIR       ; Configures P(1.7, 1.6, 1.5, 1.4, 1.2, 1.1, 1.0) as output
	mov.b #01001001b, &P1OUT       ; Sets P1.6, P1.3, P1.0 to 1 (high) and the rest to low.
	mov.b #00001000b, &P1REN       ; Enable pulldown resistor on P1.3
	mov.b #00001000b, &P1IE        ; Enable input at P1.3 as an interrupt
	mov.w #0x0049, R7              ; Writes word (73 in decimal, 0000000001001001) to register 7
	mov.b R7, &P1OUT               ; Moves 01001001 to P1OUT setting P1.6, P1.3, and P1.0 to high.
								   ; Sets P1.3 to high

	EINT                           ; Global enable interrupt. Has to be enabled for interrupt to occur when P1.3 goes from high to low.
	bis.w #CPUOFF,SR               ; Turns the bits indicated by the source on in the destination.
								   ; CPUOFF is a binary number that is a word, sets bits in status register.
								   ; If bits are set in the status register, it will stop executing 
								   ; Will just stop the code until interrupt.



PUSH:
	xor.w #0000000001000001b, R7   ; XORs 0000000001000001 with value stored in register 7
	mov.b R7,&P1OUT                ; Writes value stored in register 7 to output (turns LEDs on or off)
	bic.b #00001000b, &P1IFG       ; bitclear in P1interrupt flag register. Clearing the bit that holds the interrupt flag
								   ; So it doesn't try to deal with it again
	reti                           ; return from interrupt

	org 0xffe4                     ; This is the location in memory that the chip after you push the button P1.3
	dw PUSH
	org 0xfffe                     ; This is the location in memory that the chip goes to when you start or hit or the rest.
	dw RESET

; Note: When you assemble the code and you load it onto the chip
; This code goes into the locations specified by org
; When I turn this into a hex file and load it on, it takes reset/push code and sticks it at.
; The stuff at the bottom gets read in first, its not sequential; the code is loaded into memory
; Even if the program doesn't run all the way too the bottom.
