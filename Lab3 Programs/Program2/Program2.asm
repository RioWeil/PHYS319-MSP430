	.file	"Program2.c"
.text
	.balign 2
	.global	main
	.type	main, @function
main:
; start of function
; framesize_regs:     0
; framesize_locals:   0
; framesize_outgoing: 0
; framesize:          0
; elim ap -> fp       2
; elim fp -> sp       0
; saved regs:(none)
	; start of prologue
	; end of prologue
	MOV.W	#23168, &WDTCTL
	MOV.B	#-9, &P1DIR
	MOV.B	#73, &P1OUT
	MOV.B	#8, &P1REN
	MOV.B	#8, &P1IE
 ; 22 "Program2.c" 1
	bis.w #248, SR
 ; 0 "" 2
	; start of epilogue
	.refsym	__crt0_call_exit
	RET
	.size	main, .-main
	.balign 2
	.global	PORT1_ISR
	.section	__interrupt_vector_3,"ax",@progbits
	.word	PORT1_ISR
	.text
	.type	PORT1_ISR, @function
PORT1_ISR:
; start of function
; attributes: interrupt 
; framesize_regs:     0
; framesize_locals:   0
; framesize_outgoing: 0
; framesize:          0
; elim ap -> fp       2
; elim fp -> sp       0
; saved regs:(none)
	; start of prologue
	; end of prologue
	XOR.B	#65, &P1OUT
	BIC.B	#8, &P1IFG
	; start of epilogue
	RETI
	.size	PORT1_ISR, .-PORT1_ISR
	.ident	"GCC: (Mitto Systems Limited - msp430-gcc 8.3.0.16) 8.3.0"
