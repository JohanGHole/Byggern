#define OLED_HEIGHT 64
#define OLED_WIDTH 128
#define OLED_PAGES (OLED_HEIGHT/8)
#define FONT_OFFSET 32
#include <stdint.h>



void oled_write_c(char command);

void oled_init(void);

void oled_goto_page(uint8_t my_page);

void oled_reset(void);

void oled_home(void);

void oled_goto_column(uint8_t my_column);

void oled_clear_page(uint8_t my_page);

void oled_pos(uint8_t row, uint8_t my_column);

void oled_print(char letter);

void oled_test();

void oled_draw_matrix();

void oled_set_width(uint8_t width);

void oled_draw_smiley(uint8_t row, uint8_t column);

void oled_printf(const char* fmt, ...);

char oled_get_address_data();