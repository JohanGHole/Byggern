/*
 * spi.c
 *
 * Created: 10.10.2018 13:30:44
 *  Author: johangh
 */ 
#include "bit_manipulation.h"
#include <avr/io.h>
#include <stdint.h>
#include "SPI.h"
void SPI_MasterInit(void){
	/* Set MOSI, SCK and SS as output */
	set_bit(DDRB,DDB5);
	set_bit(DDRB,DDB7);
	set_bit(DDRB,DDB4);
	/* set MISO as input */
	clear_bit(DDRB,DDB6);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	// set SS high
	set_bit(PORTB,PB4);
}

void SPI_MasterTransmit(uint8_t cData){
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	loop_until_bit_is_set(SPSR,SPIF);
}

char SPI_MasterRecieve(void){
	SPDR = 0x00;
	loop_until_bit_is_set(SPSR,SPIF);
	return SPDR;
}

void SPI_slave_on(){
	clear_bit(PORTB,PB4);
}

void SPI_slave_off(){
	set_bit(PORTB,PB4);
}

void SPI_reset(){
	SPI_slave_on();
	SPI_MasterTransmit(0xC0);
	SPI_slave_off();
}

