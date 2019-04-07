/*
 * Midterm_Stevenlee_T2.c
 *
 * Created: 4/6/2019 7:34:18 PM
 * Author : StevenChews
 */ 

#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void usart_send (unsigned char ch);

int main (void)
{
	TCCR1B |= (1 << CS12) | (1 << CS10); // Sets prescaler to 1024
	TIMSK1 = (1 << TOIE1); // Enables overflow flag
	TCNT1 = 49911;
	sei();
	
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD-1;
	
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|    // AVcc - external cap at AREF
	(0<<ADLAR)|    // ADC Left Adjust Result
	(0<<MUX2)|     // Analog Channel Selection Bits
	(0<<MUX1)|     // ADC4 (PC4 PIN27)
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|    // ADC ENable
	(0<<ADSC)|     // ADC Start Conversion
	(0<<ADATE)|    // ADC Auto Trigger Enable
	(0<<ADIF)|     // ADC Interrupt Flag
	(0<<ADIE)|     // ADC Interrupt Enable
	(1<<ADPS2)|    // ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
	
	while (1) {}
	return 0;
}

ISR (TIMER1_OVF_vect) {
	ADCSRA|=(1<<ADSC);
	while ((ADCSRA&(1<<ADIF)) == 0);
	
	ADCSRA |= (1<<ADIF);
	
	int a = ADCL;
	a = a | (ADCH<<8);
	a = (a/1024.0) * 5000/10;
	usart_send((a/100)+'0');
	a = a % 100;
	usart_send((a/10)+'0');
	a = a % 10;
	usart_send((a)+'0');
	usart_send('\r');
	
	_delay_ms(100);
	TCNT1 = 49911; 
}

void usart_send (unsigned char ch)
{
	while (! (UCSR0A & (1<<UDRE0)));
	UDR0 = ch;
}