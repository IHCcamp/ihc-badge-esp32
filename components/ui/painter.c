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

void painter_draw_pixel(void *hwcontext, int x, int y, int color)
{
    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    int pixel_byte = ((y / 8) * WIDTH) + x;
    int pixel_bit = (y % 8);

    if (color == PAINTER_WHITE) {
        uint8_t block = ~(1 << pixel_bit);
        fb[pixel_byte] &= block;

    } else if (color == PAINTER_BLACK) {
        uint8_t block = 1 << pixel_bit;
        fb[pixel_byte] |= block;
    }
}

void painter_draw_v_line(void *hwcontext, int x, int y, int height, int color)
{
    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    if (color == PAINTER_WHITE) {
        for (int i = y; i < y + height; i++) {
            int pixel_byte = ((y / 8) * WIDTH) + x;
            int pixel_bit = (y % 8);
            uint8_t block = ~(1 << pixel_bit);

            fb[pixel_byte] &= block;
        }
    } else if (color == PAINTER_BLACK) {
        for (int i = y; i < y + height; i++) {
            int pixel_byte = ((i / 8) * WIDTH) + x;
            int pixel_bit = (i % 8);
            uint8_t block = 1 << pixel_bit;

            fb[pixel_byte] |= block;
        }
    }
}

void painter_draw_rect(void *hwcontext, int x, int y, int width, int height, int color)
{
    painter_draw_h_line(hwcontext, x, y, width, color);
    painter_draw_h_line(hwcontext, x, y + height - 1, width, color);
    painter_draw_v_line(hwcontext, x, y, height - 1, color);
    painter_draw_v_line(hwcontext, x + width - 1, y, height - 1, color);
}
