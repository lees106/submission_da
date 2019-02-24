/* 
 * Steven Lee, 5003887117, Created 2/23/2019
 * CPE 301 Design Assignment 1B
 */

.include <m328pdef.inc>

.SET STARTADDS = 0x0100
.ORG 0
	LDI XL,  LOW(STARTADDS<<1) ;XL = 00
	LDI XH, HIGH(STARTADDS<<1) ;XL = 02
	
	LDI R20, 0		;R20 = 0 (Used for ZL)
	LDI R19, 0		;R19 = 0 (Used for YL)

	LDI R17, 99		;R17 = 99 (counter for 99 numbers)
	LDI R16, 11		;R16 = 11 (starting value)

L1: 
	MOV R18, R16	;R18 = R16, (for storing value into Y if div by 3)
	MOV R22, R16	;R18 = R16, (for storing value into Z if not div by 3)

LOOP:				;Continually subtracts R22 until zero or negative, then branch.
	SUBI R22, 3		; R22 = R22 - 3
	TST R22			; Test if R22 is zero or negative
	BREQ DIV3		; Branch if R22 = 0
	BRMI NOT_DIV3	; Branch if R22 is negative
	JMP LOOP		; Jump to loop label

DIV3:				;Divisble by 3, store number
	LDI YL, LOW(0x0400)	 ;YL = 0x00
	LDI YH, HIGH(0x0400) ;YH = 0x04
	ADD YL, R19		;YL = R19
	ST Y, R18		;Y = R18
	INC R19			;R19++
	JMP CONTINUE	;Jump to continue label

NOT_DIV3:			;Not divisible by 3, store number
	LDI ZL, LOW(0x0600)  ;ZL = 0x00
	LDI ZH, HIGH(0x0600) ;ZH = 0x06
	ADD ZL, R20		;ZL = R20
	ST Z, R18		;Z = R18
	INC R20			;R20++

CONTINUE:	
	ST X+, R16		;X = R16, then increment X
	INC R16			;R16++
	DEC R17			;R17++
	BRNE L1			;Loop back to L1 Label

end:	jmp end