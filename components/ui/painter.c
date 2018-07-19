#include "painter.h"
#include "hwcontext.h"

#include <stdint.h>

#define WIDTH 84
#define HEIGHT 48

void painter_draw_h_line(void *hwcontext, int x, int y, int width, int color)
{
    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    int pixel_byte = ((y / 8) * WIDTH) + x;
    int pixel_bit = (y % 8);

    if (color == PAINTER_WHITE) {
        uint8_t block = ~(1 << pixel_bit);

        for (int i = 0; i < width; i++) {
            fb[pixel_byte + i] &= block;
        }
    } else if (color == PAINTER_BLACK) {
        uint8_t block = 1 << pixel_bit;

        for (int i = 0; i < width; i++) {
            fb[pixel_byte + i] |= block;
        }
    }
}
