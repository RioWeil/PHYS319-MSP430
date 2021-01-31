.include "msp430g2553.inc"
org 0xc000
START:                              ;This is a label we'll use later
                                    ;to refer to this position
      mov #0x0400, SP               ;Initialize the stack pointer
      mov.w #WDTPW|WDTHOLD, &WDTCTL ;Disable the watchdog, the symbol |
            						;indicates that values of WDTPW and WDTHOLD are bitwise OR'd
      mov.b #11110111b, &P1DIR      ;Set all Port 1 pins, except 1.3 as output

      mov.b #10010111b, &P1OUT      ;Sets D3-H, D2-L, D1-L, D0-H, A1-H, A0-H, and strobe-H (9 on first display)
      mov.b #10010110b, &P1OUT      ;Sets strobe-L to update the board (P1.0 to 0)
      mov.b #10010111b, &P1OUT      ;Sets strobe-H again. (P1.0 to 1)

      mov.b #00110101b, &P1OUT      ;Sets D3-L, D2-L, D1-H, D0-H, A1-H, A0-H, and strobe-H (3 on second display)
      mov.b #00110100b, &P1OUT      ;Sets strobe-L to update the board
      mov.b #00110101b, &P1OUT      ;Sets strobe-H

      mov.b #10010011b, &P1OUT      ;Sets D3-H, D2-L, D1-L, D0-H, A1-L, A0-H, and strobe-H (9 on third display)
      mov.b #10010010b, &P1OUT      ;Sets strobe-L to update the board
      mov.b #10010011b, &P1OUT      ;Sets strobe-H

      mov.b #01000001b, &P1OUT      ;Sets D3-L, D2-H, D1-L, D0-L, A1-L, A0-L, and strobe-H (4 on fourth display)
      mov.b #01000000b, &P1OUT      ;Sets strobe-L to update the board
      mov.b #01000001b, &P1OUT      ;Sets strobe-H

      mov.b #00000001b, &P1OUT      ;Sets all the pins to low (except strobe)
     

      bis.w   #CPUOFF,SR           ;Disable the CPU (end program)
org 0xfffe						   ;Load the reset vector with the
	dw START					   ;location of the program start
								   ; after power up or reset.