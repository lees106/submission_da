/* 
 * Steven Lee, 5003887117, Created
 * CPE 301 Design Assignment 1
 * Multiplication with 16-bit multiplicand and 8-bit multiplier
 * Result limits: 24-bits
 */

.include<m328pdef.inc>

.org 0x00	
	
	ldi r25,0xFF ; Load R25 = 0xFF (High byte Multiplicand)
	ldi r24,0xFF ; Load R24 = 0xFF (Low  byte Multiplicand)
	ldi r22,0xFF ; Load R22 = 0xFF (Multiplier)
	ldi r17,0x00 ; Load R17 = 0x00 (Zero constant)


L1: add r18,r24 ; Add R18 = R18 + R24
	adc r19,r25 ; Add R19 = R19 + R25 + Carry
	adc r20,r17 ; Add R20 = R20 + R17 + Carry
	DEC r22		; Decrement R22 (Multiplier)
	BRNE L1		; R22 != 0 ? Loop again L1 : Exit

end: rjmp end