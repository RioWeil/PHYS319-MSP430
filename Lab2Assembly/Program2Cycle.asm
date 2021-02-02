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
	mov.b R7, &P1OUT               ; Moves 01001001 to P1OUT setting P1.6, P1.3, and P1.0 to high
	mov.w #0000000000000001b, R8   ; Store 1 (binary, word) in register 8
	mov.w #0000000001000000b, R9   ; Store 64 (binary, word) in register 9

	EINT                           ; Global enable interrupt. Has to be enabled for interrupt to occur when P1.3 goes from high to low.
	bis.w #CPUOFF,SR               ; Turns the bits indicated by the source on in the destination.
								   ; CPUOFF is a binary number that is a word, sets bits in status register.
								   ; If bits are set in the status register, it will stop executing 
								   ; Will just stop the code until
								   ; bitset
PUSH:
	xor.w R9, R8                   ; XOR register 8 with register 9, so R8 goes between 0000000000000001 and 0000000001000001
	xor.w R8, R7                   ; XORs R7 with value stored in register 8 to cycle through light states (described below)
	mov.b R7,&P1OUT                ; Writes value stored in register 7 to output
	bic.b #00001000b, &P1IFG       ; bitclear in P1interrupt flag register. Clearing the bit that holds the interrupt flag
								   ; So it doesn't try to deal with it again
	reti                           ; return from interrupt
	org 0xffe4                     ; This is the location in memory that the chip after you push the button P1.3
	dw PUSH
	org 0xfffe                     ; This is the location in memory that the chip goes to when you start or hit or the rest.
	dw RESET

; Cycle of light states/bytes stored in registers
; CYCLE             R8Byte                 R7Byte (output)
;  0 (start)        00000001               01001001 (both on)
;  1                01000001               00001000 (both off)
;  2                00000001               00001001 (red on)  
;  3                01000001               01001000 (green on)
;  4                00000001               01001001 (both on)
; Repeats... at each stage, R8 is updated, and the previous value in R7 in XORd with value in R8 to update the value in R7.          
