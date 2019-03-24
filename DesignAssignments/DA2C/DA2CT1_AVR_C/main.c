/*
 * Steven Lee - 5003887117
 * CPE301 - Design Assignment 2C
 */

#include <avr/io.h>


int main(void)
{
    uint8_t OVFCount = 0;
	DDRB |= (1<<2) | (1<<5);
	PORTB |= (1<<5);
	TCNT0 = 0x78;
	
	TCCR0B = (1<<CS02) | (1<<CS00);
	
    while (1) 
    {	if (OVFCount <= 50) {
			while ((TIFR0 & 0x01) == 0)
			;
			TCNT0 = 0x78;
			TIFR0 = 0x01;
			OVFCount++;
		
			if (OVFCount == 50) {
				PORTB ^= (1<<2);
				OVFCount++; //Exit loop
			}
		}
		
		if (OVFCount > 50) {
			while ((TIFR0 & 0x01) == 0)
			;
			TCNT0 = 0xA5;
			TIFR0 = 0x01;
			OVFCount++;
		
			if (OVFCount == 100) {
				PORTB ^= (1<<2);
				OVFCount = 0;
			}
		}
    }
}
