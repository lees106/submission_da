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
	
	TIMSK0 |= (1<<TOIE0);				//Enable TIMER 0
	sei();								//Enable global interrupt
	
	TCCR0B |= (1<<CS02) | (1<<CS00);	//Set prescaler to 1024
	
    while (1) 
    {
    }
}

ISR (TIMER0_OVF_vect) {
	if (PORTB & (1<<DDB2))
		_delay_ms(290);
	else
		_delay_ms(435);
		
	PORTB ^= (1<<DDB2);
}

