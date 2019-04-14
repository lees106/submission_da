/*
 * Steven Lee
 * CPE 301 - DA4A
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned int ADCVal;
char curr_state = 0;


int main(void) {
	
	DDRB = 0x00;
	DDRB |= (1<<DDB1);											//Output for PWM
	DDRC = 0x00;												//Input for button
	PORTC |= (1 << DDC1);										//Internal pullup for PINC1
	
	//ADC init	
	ADMUX  |= (1<<REFS0);										//Vref = AVCC
	ADCSRA |= (1<<ADEN) | (1 <<ADSC);							//Enable ADC and start conversion
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);				//Prescaler clock of 128
	ADCSRB = 0;													//Free running mode	  
	//Timer init
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);					// Clear OC1A on compare match
	
	TCCR1A |= (1 << WGM11);	
	TCCR1B |= (1 << WGM13) | (1 << WGM12);						// Fast PWM, Non-inverted.
	
	TCCR1B |= (1 << CS11);										// Prescaler = 8
	
	//Interrupt Config
	PCICR = (1 << PCIE1);										//Enable interrupt 1
	PCMSK1 = (1 << PCINT9);										//Interrupt for PINB1
	sei();														//Enable global interrupts
	
	ICR1 = 5000;
	
	while (1){}
}

ISR(PCINT1_vect)
{
	if(!(PINC & (1 << PINC1))) {
		_delay_ms(100);											// Debounces button
		while(!(PINC & (1 << PINC1)));							// Checks if button was pressed
		
		if(curr_state) {
			PORTB |= (1 << DDB1);								//Turns on motor
			
			while(!(ADCSRA & (1<<ADIF)));						//Waits for conversion to finish
			ADCVal = ADCH + (ADCL*0b11000000);					//10-bits of ADC register
			
			OCR1A = ADCVal *0.95;								//Duty Cycle
		}
		
		else if(!curr_state) {
		    PORTB &= ~(1 << DDB1);								// Turns off motor
			
			OCR1A = 0;											// Reset
		}
		
		curr_state ^= 1;										// Remembers state of button
	}
}