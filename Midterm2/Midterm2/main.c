/*
 * Steven Lee
 * 5003887117
 * CPE Final Project - Midterm 2
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "APDS9960_def.h"
#include "i2c_master.h"

#define F_CPU 16000000UL
#define BAUD 9600
#define FOSC 16000000
#define UBRREQ FOSC/16/BAUD -1

#define WRITE 0x72
#define READ 0x72

void read_temp(void);
int load( char c, FILE *stream);
FILE str_uart = FDEV_SETUP_STREAM(load, NULL , _FDEV_SETUP_WRITE);

uint16_t red, green, blue;

int main(void) {
	red = 0; 
	green = 0; 
	blue = 0;
	
	// UART Init
	uint16_t baud_rate = UBRREQ;

	UBRR0H = baud_rate >> 8;
	UBRR0L = baud_rate & 0xFF;	
	UCSR0B = ( 1 <<RXEN0)|( 1 <<TXEN0); //Enable rx and tx
	UCSR0C = (3 <<UCSZ00); // 8-bit data, 1-bit stop

	// ADPS Init
	uint8_t config;

	i2c_readReg(WRITE, APDS9960_ID, &config,1);
	if(config != APDS9960_ID_1) while(1);
	config = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	i2c_writeReg(WRITE, APDS9960_ENABLE, &config, 1);
	config = DEFAULT_ATIME;
	i2c_writeReg(WRITE, APDS9960_ATIME, &config, 1);
	config = DEFAULT_WTIME;
	i2c_writeReg(WRITE, APDS9960_WTIME, &config, 1);
	config = DEFAULT_PROX_PPULSE;
	i2c_writeReg(WRITE, APDS9960_PPULSE, &config, 1);
	config = DEFAULT_POFFSET_UR;
	i2c_writeReg(WRITE, APDS9960_POFFSET_UR, &config, 1);
	config = DEFAULT_POFFSET_DL;
	i2c_writeReg(WRITE, APDS9960_POFFSET_DL, &config, 1);
	config = DEFAULT_CONFIG1;
	i2c_writeReg(WRITE, APDS9960_CONFIG1, &config, 1);
	config = DEFAULT_PERS;
	i2c_writeReg(WRITE, APDS9960_PERS, &config, 1);
	config = DEFAULT_CONFIG2;
	i2c_writeReg(WRITE, APDS9960_CONFIG2, &config, 1);
	config = DEFAULT_CONFIG3;
	i2c_writeReg(WRITE, APDS9960_CONFIG3, &config, 1);	i2c_init();
	stdout = &str_uart;
	
	_delay_ms(5000);
	printf("AT+CWMODE=3\r\n");
	
	_delay_ms(5000);
	printf("AT+CWJAP=\"Steven Lee’s iPhone\", \"123456789\"\r\n");	
	
	while(1) { // Indefinitely send values to Thingspeak
		_delay_ms(5000);
		printf("AT+CIPMUX=0\r\n"); 		// Enable Single Connection

		_delay_ms(5000);
		printf("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");		// Start the connection to the cloud
		
		_delay_ms(5000);
		read_temp();
		
		printf("AT+CIPSEND=104\r\n");
		printf("GET https://api.thingspeak.com/update?api_key=360X1PTM88XFSF19&field1=0%05u&field2=%05u&field3=%05u\r\n", red, green, blue);
		
		_delay_ms(3000);
	}
}

void read_temp() {
	uint8_t redH, redL, greenH, greenL, blueH, blueL;
	
	i2c_readReg(WRITE, APDS9960_RDATAH, &redH, 1);
	i2c_readReg(WRITE, APDS9960_RDATAL, &redL, 1);
	i2c_readReg(WRITE, APDS9960_GDATAH, &greenH, 1);
	i2c_readReg(WRITE, APDS9960_GDATAL, &greenL, 1);
	i2c_readReg(WRITE, APDS9960_BDATAH, &blueH, 1);
	i2c_readReg(WRITE, APDS9960_BDATAL, &blueL, 1);
	
	red   = (redH << 8)   | redL;
	green = (greenH << 8) | greenL;
	blue  = (blueH << 8)  | blueL;
	
	if (red > 255)
		red = 255;
	if (green > 255)
		green = 255;
	if (blue > 255)
		blue = 255; 
}


int load(char c, FILE *stream) {
	while (!(UCSR0A & ( 1 << UDRE0))); //This will check for empty buffer.
	UDR0 = c;
	return 0;
}