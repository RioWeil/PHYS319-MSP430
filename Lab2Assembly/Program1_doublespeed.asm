.include "msp430g2553.inc"                ; Includes file defining names of addresses/bytes/words
	org 0xc000                            ; Tells assembler where to put the program in memory
start:
	mov.w #WDTPW|WDTHOLD, &WDTCTL         ; Disables the watchdog counter
	mov.b #0x41, &P1DIR                   ; 0x41 in hex is 01000001 in binary. Configs pins P1.6 and P1.0 to ouputs.
	                                      ; Setting the two LED pins to output
	mov.w #0x01, r8                       ; Moving a word (16 bits) 0000000000000001 into r8 (general purpose 16-bit register) 

repeat:
	mov.b r8, &P1OUT                      ; Move a byte (0000001 or 01000000) into P1OUT (switches P1.0 off P1.6 on or vise versa) 
	xor.b #0x41, r8                       ; Xors 01000001 with 00000001 -> produces 01000000
	mov.w #30000, r9                      ; Moves a word (16-bit) into r9 (register). Larger value = More time before LEDs switch


waiter:
	dec r9                                ; Decrements r9 by one.
	jnz waiter                            ; Jump to waiter label if zero bit is reset (and repeats the decreasing process until zero)
	jmp repeat                            ; Jump to repeat label 
	org 0xfffe                            ; Location of the memory that we go to if we hit reset
	dw start                              ; Puts a label start in memory 0xfffe so the code knows where to start location.
