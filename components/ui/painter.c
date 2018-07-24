#include "painter.h"
#include "hwcontext.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define WIDTH (84 + 4) // last pixels are discarded
#define HEIGHT 48

#define MAX_SCREEN_TEXT_ROWS (48 / 8)

#define CHECK_BOUNDS() \
    if ((x < 0) || (x + width > PAINTER_SCREEN_WIDTH) || (y < 0) ||  (y + height > PAINTER_SCREEN_HEIGHT)) { \
        fprintf(stderr, "Skipped draw\n"); \
        return; \
    }

#include "font_small_plain.c"
#include "font_small_bold.c"

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
            int pixel_byte = ((i / 8) * WIDTH) + x;
            int pixel_bit = (i % 8);
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
    CHECK_BOUNDS();

    painter_draw_h_line(hwcontext, x, y, width, color);
    painter_draw_h_line(hwcontext, x, y + height - 1, width, color);
    painter_draw_v_line(hwcontext, x, y, height - 1, color);
    painter_draw_v_line(hwcontext, x + width - 1, y, height - 1, color);
}

void painter_draw_fill_rect(void *hwcontext, int x, int y, int width, int height, int color)
{
    CHECK_BOUNDS();

    for (int x_pos = x; x_pos < x + width; x_pos++) {
        painter_draw_v_line(hwcontext, x_pos, y, height, color);
    }
}

void painter_draw_xbm(void *hwcontext, const unsigned char *img_bits, int x, int y, int width, int height)
{
    CHECK_BOUNDS();

    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    int src_width = ((width + 8 - 1) >> 3) << 3;

    for (int src_x = 0; src_x < width; src_x++) {
        for (int src_y = 0; src_y < height; src_y++) {
            int byte = (src_y * src_width + src_x) / 8;
            int bit = (src_y * src_width + src_x) % 8;
            int color = (img_bits[byte] & (1 << bit)) ? PAINTER_BLACK : PAINTER_WHITE;

            put_pixel(fb, x + src_x, y + src_y, color);
        }
    }
}

void painter_draw_bounded_text(void *hwcontext, int x, int row, int x_bound, int y_row_bound, const char *text, int style, int color)
{
    if ((row < 0) || (row > MAX_SCREEN_TEXT_ROWS)) {
        fprintf(stderr, "Skipping draw text outside bounds\n");
    }

    const int *offsets;
    const uint8_t *widths;
    const uint8_t *font_data;

    if (style == PAINTER_FONT_REGULAR) {
        offsets = font_small_plain_offsets;
        widths = font_small_plain_pixel_widths;
        font_data = font_small_plain_font_data;
    } else {
        offsets = font_small_bold_offsets;
        widths = font_small_bold_pixel_widths;
        font_data = font_small_bold_font_data;
    }

    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    int x_pos = x;
    int j = row * WIDTH + x;
    while (*text) {
        int font_index = *text - 0x20;

        int off = offsets[font_index];
        int c_width = widths[font_index];

        if (x_pos + c_width > x_bound) {
            x_pos = x;
            row++;
            j = row * WIDTH + x;

            if (row > y_row_bound) {
                fprintf(stderr, "Text has been truncated.\n");
                return;
            }
        }
        x_pos += c_width;

        for (int i = off; i < off + c_width; i++) {
            if (color == PAINTER_BLACK) {
                fb[j] |= font_data[i];
            } else {
                fb[j] &= ~(font_data[i]);
            }
            j++;
        }

        text++;
    }
}

void painter_draw_text(void *hwcontext, int x, int row, const char *text, int style, int color)
{
    painter_draw_bounded_text(hwcontext, x, row, PAINTER_SCREEN_WIDTH, MAX_SCREEN_TEXT_ROWS, text, style, color);
}

int painter_painted_text_width(const char *text, int style)
{
    const uint8_t *widths;

    if (style == PAINTER_FONT_REGULAR) {
        widths = font_small_plain_pixel_widths;
    } else {
        widths = font_small_bold_pixel_widths;
    }

    int acc = 0;
    while (*text) {
        int font_index = *text - 0x20;

        acc += widths[font_index];
        text++;
    }

    return acc;
}
