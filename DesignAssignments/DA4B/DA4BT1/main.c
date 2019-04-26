/*
 * Steven Lee 
 * CPE 301 - DA4BT1.c
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

volatile unsigned int ADCVal;
void delay_here();

int main(void) {
		
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);		// Output
	DDRC  = 0;															// Input
		
	// Timer 1
	TCCR1B = (1 << WGM12) | (1 << CS11);	   // Prescaler set to 8
	
	// ADC Init
	DIDR0   =  0x1;							        // Disable ADC0 pin
	ADMUX   =  (1 << REFS0);						// Ref is aref
	ADCSRA |=  (1 <<ADEN)   | (1 << ADPS2);      
	ADCSRA |=  (1 << ADPS1) | (1 << ADPS0);		    // Enable ADC
	ADCSRA |=  (1 << ADSC);			                // Starts conversion
	ADCSRB = 0x0;							        // FRM
	
	while(1) {
		while ((ADCSRA & (1<<ADIF)) == 0);	// Waits for conversion
		ADCVal = ADC & 0x03FF;			    // Takes right 10-bits of ADC register
		OCR1A = 10*ADCVal;				    // OCR1A value for duty cycle

		PORTB = 0x09;
		delay_here();
		
		PORTB = 0x03;
		delay_here();
		
		PORTB = 0x06;
		delay_here();
		
		PORTB = 0x0C;
		delay_here();
	}
}

void delay_here() {
	while (!(TIFR1 & (1<<OCF1A)));      // CTC delay
	TIFR1 |= (1 << OCF1A);				// Resets flag
}