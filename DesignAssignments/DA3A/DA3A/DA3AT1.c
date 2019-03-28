/*
 * Steven Lee
 * 5003887117
 * CPE 301 - DA3A
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUDRATE 9600
#define F_CPU 16000000UL
#define USART_BAUDRATE F_CPU/16/BAUDRATE-1

void USART_init( void ) {
	UBRR0L = USART_BAUDRATE;
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);  // 8-bit data
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);    // Enable RX and TX
	
	TIMSK1 = (1 << TOIE1);				 // Enables overflow flag
	TCNT1  = 49911;						 //Calc to start the timer overflow
	sei();								 //Enable interrupts
	
	TCCR1A |= 0;
	TCCR1B |= (1 << CS12) | (1 << CS10); // Sets prescaler to 1024
}

ISR (TIMER1_OVF_vect) {
	char newline_result[] = "\n";
	
	char string_result[] = "String - ";
	char string_result1[] = "My name is Steven!";
	
	USART_putstring(string_result); // LABEL PRINT "String: "
	USART_putstring(string_result1); // prints string "This is my DA3A"
	USART_putstring(newline_result); // creates next line
	
	char int_result[] = "Integer - ";
	
	USART_putstring(int_result); // LABEL PRINT "Integer: "
	USART_send('66'); // prints value 5
	USART_putstring(newline_result); // creates next line
	
	char float_result[] = "Floating Point - ";
	volatile float num = 12.0416;
	char outs[20];
	
	USART_putstring(float_result); // LABEL PRINT "FLoating Point: "
	snprintf(outs, sizeof(outs), "%f\r\n", num); // the floating point characters are stored in outs
	USART_putstring(outs); // transmits outs to UART
	USART_putstring(newline_result); // creates next line
	USART_putstring(newline_result); // creates next line

	TCNT1 = 49911; // Reset timer
}

void USART_send(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
	
}

void USART_putstring(char *Ptr) {
	while ((*Ptr != '\0')){
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = *Ptr;
		Ptr++;
	}
}

int main(void) {
	USART_init();	// Initializes A -> D
	while(1){}
}

