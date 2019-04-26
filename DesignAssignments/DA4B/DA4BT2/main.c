/*
 * DA4B.c
 *
 * Created: 4/20/2019 3:36:08 PM
 * Author : Steven
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	// Timer 1
	TCCR1A = (1 << COM1A1) | (1<<COM1B1) | (1<<WGM11);			// Noninverting Mode and Fast PWM Mode
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10);	// Prescaler 64

	// ADC init
	ADMUX = (1<<REFS0);											// Ref = Aref
	ADCSRA |= (1<<ADEN) | (1 << ADSC) | (1<<ADPS2);
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);							// Enable ADC
	ADCSRB = 0x0;												// FWM
	
	while(1) {
		ADMUX = (1<<REFS0);										
		ADCSRA |= (1<<ADEN) | (1 << ADSC) | (1<<ADPS2);
		ADCSRA |= (1<<ADPS1) | (1<<ADPS0);						
		ADCSRB = 0x0;		
		
		ICR1 = 0xFFFF;											// freq PWM = 50Hz
		DDRB |= (1<<DDB1);										// DDB1 output
		OCR1A = ADC;											// Fetch potentiometer value
		_delay_ms(30);											// Delay
	}
}
