/*
 * spi.c
 *
 * Created: 10.10.2018 13:30:44
 *  Author: johangh
 */ 
#define F_CPU 16000000
#include "bit_manipulation.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "SPI.h"
void
SPI_MasterInit(void)
{
	/* Set MOSI, SCK, SS and I/O select as output */
	set_bit(DDRB,DDB2);
	set_bit(DDRB,DDB1);
	set_bit(DDRB,DDB0);
	set_bit(DDRB,DDB7);
	/* set MISO as input */
	clear_bit(DDRB,DDB3);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	// set I/O select (active high)
	set_bit(PORTB,PB7);
	//set_bit(PORTB,PB0);
}
void SPI_MasterTransmit(uint8_t cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	loop_until_bit_is_set(SPSR,SPIF);
	_delay_ms(1);
}
char SPI_MasterRecieve(void){
	SPDR = 0x00;
	loop_until_bit_is_set(SPSR,SPIF);
	_delay_ms(1);
	return SPDR;
}
void SPI_slave_on(){
	clear_bit(PORTB,PB7);
}
void SPI_slave_off(){
	set_bit(PORTB,PB7);
}

void SPI_reset(){
	SPI_slave_on();
	SPI_MasterTransmit(0xC0);
	printf("reset transmit \n");
	SPI_slave_off();
}

void SPI_test(){
	SPI_reset();
	SPI_slave_on();
	SPI_MasterTransmit(0x03);
	SPI_MasterTransmit(0x0E);	
	uint8_t some_data = SPI_MasterRecieve();
	SPI_slave_off();
	printf("this should now be 0x80: %d\n",some_data);
}