#include "st7789.h"

int main(void)
{
    st7789_pico_port_init();

    ST7789_Init();
    ST7789_TestColors();
    ST7789_Fill_Color(BLACK);

    while (true) {
        ST7789_Test();
    }
}