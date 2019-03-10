/*
* Steven Lee
* 5003887117
* CPE 301 - Design Assignment 2B
* Created 03/09/2019
*/

.include <m328pdef.inc>
.org 0

jmp main
.org 0x02
jmp EX0_ISR

main:
	LDI R20,HIGH(RAMEND)
	OUT SPH,R20
	LDI R20,LOW(RAMEND) 
	OUT SPL, R20
	LDI R16, 0xFF

	LDI R20, 0x2
	STS EICRA, R20

	SBI DDRB, 2
	SBI PORTD, 2
	LDI R20, 1<<INT0

	OUT EIMSK, R20
	SEI

HERE: 
	OUT PORTB, R16
	JMP HERE

EX0_ISR:
	IN R21, PORTB
	LDI R22, (1<<2)
	EOR R21, R22
	OUT PORTB, R21
	CALL delay
	RETI

delay:
	LDI R20, 30
D0:
	LDI R21, 160
D1:
	LDI R22, 250
D2:
	NOP
	NOP
	DEC R22
	BRNE D2
	DEC R21
	BRNE D1
	DEC R20
	BRNE D0
	RET
