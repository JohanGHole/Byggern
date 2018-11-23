#include <stdint.h>
#include "adc.h"

typedef struct {
	uint8_t left;
	uint8_t right;
} slider_position;


void slider_init(void);

uint8_t slider_left_button(void);

uint8_t slider_right_button(void);

slider_position slider_find_position(void);

