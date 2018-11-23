#define address_sram ((volatile char*)0x1800)
#define address_adc ((volatile char*)0x1400)
#define address_oled_data ((volatile char*)0x1200)
#define address_oled_cmd ((volatile char*)0x1000)
#include <avr/io.h>

void memory_init();