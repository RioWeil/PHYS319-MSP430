.include "msp430g2553.inc"
org 0xc000
START:                              ;This is a label we'll use later
                                    ;to refer to this position
      mov #0x0400, SP               ;Initialize the stack pointer
      mov.w #WDTPW|WDTHOLD, &WDTCTL ;Disable the watchdog
      mov.b #11110111b, &P1DIR      ;Set all Port 1 pins, except 1.3 as output

      ; This block of code just to initialize the display to 4 zeros (blank)
      mov.b #00001111b, &P1OUT      ; Sets first display to 0
      mov.b #00001110b, &P1OUT      ; Sets strobe-L to update the board (P1.0 to 0)
      mov.b #00001111b, &P1OUT      ; Sets strobe-H again. (P1.0 to 1)

      mov.b #00001101b, &P1OUT      ; Sets second display to 0
      mov.b #00001100b, &P1OUT      ; Sets strobe-L to update the board
      mov.b #00001101b, &P1OUT      ; Sets strobe-H

      mov.b #00001011b, &P1OUT      ; Sets third display to 0
      mov.b #00001010b, &P1OUT      ; Sets strobe-L to update the board
      mov.b #00001011b, &P1OUT      ; Sets strobe-H

      mov.b #00001001b, &P1OUT      ; Sets fourth display to 0
      mov.b #00001000b, &P1OUT      ; Sets strobe-L to update the board
      mov.b #00001001b, &P1OUT      ; Sets strobe-H

      mov.b #00001000b, &P1REN       ; Enable pulldown resistor on P1.3
      mov.b #00001000b, &P1IE        ; Enable input at P1.3 as an interrupt
     

      mov.w #0000000000001001b, R7           ; Binary code to set P1.0 (strobe) to high, P1.1-P1.2 (A0-A1) to low, and 0000 to P1.7-P1.4 (D3-D0)
      mov.w #0000000000001000b, R8           ; Same as R8, but P1.0 set to low (For switching down the strobe). These two control first digit.
      mov.w #0000000000001011b, R9           ; Binary code to set P1.0 (strobe) to high, P1.2 (A1) high, P1.1 (A0) low. 0000 to P1.7-P1.4 (D3-D0)
      mov.w #0000000000001010b, R10          ; Same as R9, but P1.0 set to low (For switching down the strobe). These two control second digit.
      mov.w #0000000000001101b, R11          ; Same as above, but P1.2 low, P1.1 high
      mov.w #0000000000001100b, R12          ; Same as R11, but P1.0 set to low (For switching down the strobe). These two control third digit.
      mov.w #0000000000001111b, R13          ; Same as above, but P1.2-P1.1 both high
      mov.w #0000000000001110b, R14          ; Same as R13, but P1.0 set to low (For switching down the strobe). These two control fourth digit.

      mov.w #0000000000010000b, R15          ; Stores 000100000 in register 15; this will be used to increment the counter when we press button.



      EINT                           ; Global enable interrupt. Has to be enabled for interrupt to occur when P1.3 goes from high to low.
      bis.w #CPUOFF,SR               ; Turns the bits indicated by the source on in the destination.
                                     ; CPUOFF is a binary number that is a word, sets bits in status register.
                                     ; If bits are set in the status register, it will stop executing 

PUSH: 
      add.b R15, R7
      add.b R15, R8         ; Incremenets R7, R8 by one using value in R15

      cmp.b #10101001b, R7
      jeq disp2             ; If comparison is true (i.e. register holds value greater than 9) then go to function that sets 2nd display

      mov.b R7, &P1OUT      ; Update first display
      mov.b R8, &P1OUT      ; Sets strobe-L to update the board (P1.0 to 0)
      mov.b R7, &P1OUT      ; Sets strobe-H again. (P1.0 to 1)

      bic.b #00001000b, &P1IFG       ; bitclear in P1interrupt flag register. Clearing the bit that holds the interrupt flag
      reti                           ; return from interrupt

disp2:
      mov.w #0000000000001001b, R7   ; Reset register 7 to initial state (zero)
      mov.w #0000000000001000b, R8   ; Reset register 8 to initial state (zero)
      mov.b R7, &P1OUT      ; Sets first display to 0
      mov.b R8, &P1OUT      ; Sets strobe-L to update the board (P1.0 to 0)
      mov.b R7, &P1OUT      ; Sets strobe-H again. (P1.0 to 1)

      add.b R15, R9
      add.b R15, R10         ; Increments R9, R10 by one using value in R15

      cmp.b #10101011b, R9
      jeq disp3              ; If comparison is true (i.e. register holds value greater than 9) then go to function that sets 3rd display

      mov.b R9, &P1OUT       ; Update second display
      mov.b R10, &P1OUT      ; Sets strobe-L to update the board (P1.0 to 0)
      mov.b R9, &P1OUT       ; Sets strobe-H again. (P1.0 to 1)

      bic.b #00001000b, &P1IFG       ; bitclear in P1interrupt flag register. Clearing the bit that holds the interrupt flag
      reti                           ; return from interrupt

disp3:
      mov.w #0000000000001011b, R9           ; Reset register 9 to initial state (zero)
      mov.w #0000000000001010b, R10          ; Reset register 10 to initial state (zero)
      mov.b R9, &P1OUT      ; Sets second display to 0
      mov.b R10, &P1OUT     ; Sets strobe-L to update the board (P1.0 to 0)
      mov.b R9, &P1OUT      ; Sets strobe-H again. (P1.0 to 1)

      add.b R15, R11
      add.b R15, R12         ; Increments R11, R12 by one by using value in R15

      cmp.b #10101101b, R11
      jeq disp4              ; If comparison is true (i.e. register holds value greater than 9) then go to function that sets 4th display

      mov.b R11, &P1OUT      ; Update third display
      mov.b R12, &P1OUT      ; Sets strobe-L to update the board (P1.0 to 0)
      mov.b R11, &P1OUT       ; Sets strobe-H again. (P1.0 to 1)

      bic.b #00001000b, &P1IFG       ; bitclear in P1interrupt flag register. Clearing the bit that holds the interrupt flag
      reti                           ; return from interrupt

disp4:
      mov.w #0000000000001101b, R11          ; Reset register 11 to initial state (zero)
      mov.w #0000000000001100b, R12          ; Reset register 12 to initial state (zero)
      mov.b R11, &P1OUT      ; Sets third display to 0
      mov.b R12, &P1OUT     ; Sets strobe-L to update the board (P1.0 to 0)
      mov.b R11, &P1OUT      ; Sets strobe-H again. (P1.0 to 1)

      add.b R15, R13
      add.b R15, R14         ; Increments R13, R14 by one by using value in R15

      cmp.b #10101111b, R13
      jeq START              ; Go back to start (reset everything) if we hit 9999

      mov.b R13, &P1OUT      ; Update fourth display
      mov.b R14, &P1OUT      ; Sets strobe-L to update the board (P1.0 to 0)
      mov.b R13, &P1OUT      ; Sets strobe-H again. (P1.0 to 1)


org 0xffe4                     ; This is the location in memory that the chip after you push the button P1.3
      dw PUSH
org 0xfffe						   ;Load the reset vector with the
	dw START					   ;location of the program start after power up or reset
