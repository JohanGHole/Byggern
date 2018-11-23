/*
 * SPI2.h
 *
 * Created: 06.11.2018 15:10:52
 *  Author: Johangh
 */ 

#include <stdint.h>
#pragma once

void SPI_chipselect(uint8_t enable);

void SPI_write(uint8_t c);

uint8_t SPI_read(void);

void SPI_test2();