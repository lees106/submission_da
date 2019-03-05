/* 
 * Steven Lee, 5003887117, Created 3/4/2019
 * CPE 301 Design Assignment 2AT1
 */

#include <avr/io.h>
#include <stdio.h>

int main(void) {
	DDRB = (1<<2);	// Set direction register portb2 as output
	TCCR1B = 5;		// Set the prescaler to 1024
	
	while(1) {
		TCNT1 = 0;	// Set time counter to 0
		while (TCNT1 < 6796){}	// Time counter will count to 1A8C, which complies to 60% DC
		PORTB ^= (1<<2);		// Turn on LED
		TCNT1 = 0;	// Reset time counter to 0
		while (TCNT1 < 4530){}	// Time counter will count to 11B2, which is the remaining time
		PORTB ^= (1<<2); // Turn off LED
	}
	
	return 0;
}

