	.file	"Program1.c"
.text
	.balign 2
	.global	main
	.type	main, @function
main:
; start of function
; framesize_regs:     0
; framesize_locals:   2
; framesize_outgoing: 0
; framesize:          2
; elim ap -> fp       2
; elim fp -> sp       2
; saved regs:(none)
	; start of prologue
	SUB.W	#2, R1
	; end of prologue
	MOV.W	#23168, &WDTCTL
	MOV.B	#65, &P1DIR
	MOV.B	#1, &P1OUT
	MOV.W	#-5536, R12
.L4:
	MOV.W	R12, @R1
.L2:
	MOV.W	@R1, R13
	CMP.W	#0, R13 { JNE	.L3
	XOR.B	#65, &P1OUT
	BR	#.L4
.L3:
	ADD.W	#-1, @R1
	BR	#.L2
	.size	main, .-main
	.ident	"GCC: (Mitto Systems Limited - msp430-gcc 8.3.0.16) 8.3.0"
