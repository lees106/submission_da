/*
 * Midterm_Stevenlee.c
 *
 * Created: 4/6/2019 3:02:41 PM
 * Author : Steven Lee
 */ 

#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <util/delay.h>

#include <stdlib.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD 115200 
#define FOSC 16000000
#define MY_UBR FOSC/8/BAUD -1

volatile uint8_t ADCvalue;
volatile unsigned char ADCtemp[5];

volatile unsigned char CWMODE[] = "AT+CWMODE=3\r\n"; 
volatile unsigned char CWJAP[] = "AT+CWJAP=\"Steven Lee’s iPhone\", \"120412041204\"\r\n";
volatile unsigned char CIPMUX[] = "AT+CIPMUX=0\r\n"; 

volatile unsigned char CIPSTART[] = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n";
volatile unsigned char CIPSEND[] = "AT+CIPSEND=45\r\n"; 
volatile unsigned char FETCH[] = "GET /update?key=FLE3VYRA30047RIG&field1="; 
volatile unsigned char PAUSE[] = "\r\n\r\n";

void send_AT (volatile unsigned char c[]); //sends the arrays to this function

 int main (void) {
	 /* ADC */
	 ADMUX = 0;				// use ADC0 or PC0
	 ADMUX |= (1 << REFS0); // use AVcc as the ref
	 ADMUX |= (1 << ADLAR); // Right adjust
	 
	 ADCSRA |= (1 << ADATE); // Auto Trigger Enable
	 ADCSRB = 0;             // free running mode
	 ADCSRA |= (1 << ADEN);  // Enable the ADC
	 ADCSRA |= (1 << ADIE);  // Enable Interrupts
	 ADCSRA |= (1 << ADSC);  // Start the ADC conversion
	 ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescaler for 16Mhz
	 
	
	 /* UART */
	 UBRR0H = ((MY_UBR) >> 8); // Store upper bits
	 UBRR0L = MY_UBR;          // Store lower bits

   	 UCSR0A |= (1 << U2X0);    // Double USART transmission speed
	 UCSR0B |= (1 << TXEN0);   // Enable transmitter
	 UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // 8 data, 1 stop, no parity
	 
	 _delay_ms(1000);
	 send_AT(CWMODE);
	 _delay_ms(1000); 
	 send_AT(CWJAP);
	 _delay_ms(2000); 
	 send_AT(CIPMUX);
	 	  
	 sei();	

	 while(1) {
		_delay_ms(1000); 
		 send_AT(CIPSTART); // Connect to thingspeak, port 80
		 _delay_ms(1000);
		 send_AT(CIPSEND);
		 _delay_ms(1000); 
		 send_AT(FETCH);
		 send_AT(ADCtemp);   // Sends the temperature data
		 send_AT(PAUSE);     // Break in the data display
	 }
 }
 
 
ISR(ADC_vect) {
    volatile unsigned int j=0;
	char temp[5];
	 
	ADCvalue = (ADCH << 1);
	itoa(ADCvalue, temp, 10);
	
	while (j<5) {
		ADCtemp[j] = temp[j];
		j++;
	}
 }


void send_AT(volatile unsigned char c[]) {
	volatile unsigned int i=0, j=0;
	
	//Counts the number of chars until end of string.
	while (c[j] != 0x00) {
		j++; 
	}
	
	//Waits for UDRE0 flag to be high, meaning buffer is empty and ready for writing.
	while (i<j) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = c[i];
		i++;
	}
}
