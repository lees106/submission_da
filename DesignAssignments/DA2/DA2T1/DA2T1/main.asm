/* 
 * Steven Lee, 5003887117, Created 3/4/2019
 * CPE 301 Design Assignment 2AT1
 */


.include <m328pdef.inc>
.CSEG
.org 0
 LDI R16,0b0000_0100

 OUT DDRB, R16

 LDI R17,0

 LDI R20,5
 STS TCCR1B,R20

begin:
 LDI R20, 0b0000_0000
 STS TCNT1H,R20
 STS TCNT1L,R20

 RCALL delay_one
 OUT PORTB,R17

 LDI R20, 0b0000_0000
 STS TCNT1H,R20
 STS TCNT1L,R20

 RCALL delay_two
 OUT PORTB,R17
 RJMP begin


delay_one:
 LDS R29, TCNT1H
 LDS R28, TCNT1L

 CPI R28,0x8C
 BRSH body
 RJMP delay_one


body:
 CPI R29,0x1A
 BRLT delay_one
 RET


delay_two:
 LDS R29, TCNT1H
 LDS R28, TCNT1L

 CPI R28,0xB2
 BRSH body_two
 RJMP delay_two


body_two:
 CPI R29,0x11
 BRLT delay_two
 RET