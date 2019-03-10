#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void) {
	DDRB  |= (1<<5) | (1<<2); // Set PB5 and PB2 as output
	PORTB |= (1<<5);		  // Set PB5 high to turn off LED
	PORTD |= 1<<2;			  // Enable Pull Up on PD2
	
	EICRA = 0x2;			  // Make INTO falling edge triggered
	EIMSK = (1<<INT0);		  // Enable external INT0
    
	sei();				      //Enables global interrupt

    while (1) {};
}

ISR (INT0_vect) {			  // ISR for external INT0
    PORTB ^= (1<<2);		  //Toggle LED
    _delay_ms(1250);
}