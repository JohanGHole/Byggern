#include "UART.h"
#include <avr/io.h>
#include <stdio.h>

#define FOSC 16000000UL// Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

static FILE uart_io = FDEV_SETUP_STREAM(uart_send, uart_read, _FDEV_SETUP_RW);

void uart_init(void){
	
	UBRR0H = (unsigned char)((MYUBRR)>>8);
	UBRR0L = (unsigned char) MYUBRR; 
	/* enable receiver (RDX0) and transmitter (TXD0) */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); 
	/* frame format set to 8data, 2stop bit */
	UCSR0C =  (3 << UCSZ00) | (1 << USBS0);
	
	fdevopen(uart_send, NULL);
	
}

void uart_send(unsigned char data){
	//Wait for empty transmit buffer
	while(!(UCSR0A & (1 << UDRE0))){ //UDRE0 = Data Register Empty, high when no data
		; //checking the Data Bus Empty flag in the control-register
	}
	//Put data into buffer, sends the data
	UDR0 = data;
}

unsigned char uart_read(void) {
	//Wait for data to be received
	while ( !(UCSR0A & (1 << RXC0))){
		; //checking the receive complete (RXC) flag in the control-register. 
	}
	//Get and return received data from buffer
	return UDR0;
}

void uart_printf(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	vfprintf(&uart_io, fmt, args);
	va_end(args);
}