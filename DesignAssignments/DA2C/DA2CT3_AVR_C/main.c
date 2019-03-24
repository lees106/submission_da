/*
 * Steven Lee - 5003887117
 * CPE301 - Design Assignment 2C
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int main(void)
{
	DDRB   |= (1<<DDB2) | (1<<DDB5);	//Set PINB2 and PINB5 as output
	PORTB  |= (1<<DDB5);				//Clear PINB5 so LED turns off
	OCR0A  = 0;							//Load compare register value
	TCCR0A |= (1<< WGM01);				//Set CTC mode
	TIMSK0 |= (1<< OCIE0A);				//Set interrupt on compare match
	TCCR0B |= (1<<CS02) | (1<<CS00);	//Set pre-scaler to 1024
	sei();								//Enable interrupt

	
    while (1) 
    {
    }
}

ISR (TIMER0_COMPA_vect) {
	if (PORTB & (1<<DDB2))
		_delay_ms(290);
	else
		_delay_ms(435);
		
	PORTB ^= (1<<DDB2);
}

