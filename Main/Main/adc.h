#pragma once
#include <stdint.h>

// addresses for configuring the ADC-channels
typedef enum {
	CH_X = 4,
	CH_Y = 5,
	CH_LEFT_SLIDER = 6,
	CH_RIGHT_SLIDER = 7
}ADC_CHANNEL;

typedef struct
{
	int x;
	int y;
}position;

void ADC_init(void);

uint8_t ADC_read(ADC_CHANNEL ch);

