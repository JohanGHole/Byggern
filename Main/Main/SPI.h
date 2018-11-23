/*
 * spi.h
 *
 * Created: 10.10.2018 13:31:01
 *  Author: johangh
 */ 


#ifndef SPI_H_
#define SPI_H_

void SPI_MasterInit(void);

void SPI_MasterTransmit(uint8_t cData);

char SPI_MasterRecieve(void);

void SPI_slave_on(void);

void SPI_slave_off(void);


#endif /* SPI_H_ */