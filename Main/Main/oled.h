#define OLED_HEIGHT 64
#define OLED_WIDTH 128
#define OLED_PAGES (OLED_HEIGHT/8)
#define FONT_OFFSET 32
#include <stdint.h>

void oled_write_c(char command);

void oled_init(void);

void oled_goto_page(uint8_t my_page);

void oled_reset(void);

void oled_goto_column(uint8_t my_column);

void oled_clear_page(uint8_t my_page);

void oled_update_buffer(uint8_t row, uint8_t column);

void oled_buffer_init();

void oled_buffer_display();

void oled_print(char letter);

void set_width(uint8_t width);

void oled_printf(const char* fmt, ...);

void oled_buffer_rectangle(uint8_t posX,uint8_t posY, uint8_t lengthX, uint8_t lengthY,uint8_t fill);