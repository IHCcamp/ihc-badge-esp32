#include "painter.h"
#include "hwcontext.h"

#include <string.h>
#include <stdint.h>

#define WIDTH (84 + 4) // last pixels are discarded
#define HEIGHT 48

static inline void put_pixel(uint8_t *fb, int x, int y, int color)
{
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

void painter_clear_screen(void *hwcontext)
{
    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    memset(fb, 0, (WIDTH * HEIGHT) / 8);
}

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
    put_pixel(fb, x, y, color);
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

void painter_draw_fill_rect(void *hwcontext, int x, int y, int width, int height, int color)
{
    for (int x_pos = x; x_pos < x + width; x_pos++) {
        painter_draw_v_line(hwcontext, x_pos, y, height, color);
    }
}

void painter_draw_xbm(void *hwcontext, const unsigned char *img_bits, int x, int y, int width, int height)
{
    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    int src_width = width + (8 - (width % 8));

    for (int src_x = 0; src_x < width; src_x++) {
        for (int src_y = 0; src_y < height; src_y++) {
            int byte = (src_y * src_width + src_x) / 8;
            int bit = (src_y * src_width + src_x) % 8;
            int color = (img_bits[byte] & (1 << bit)) ? PAINTER_BLACK : PAINTER_WHITE;

            put_pixel(fb, x + src_x, y + src_y, color);
        }
    }
}
