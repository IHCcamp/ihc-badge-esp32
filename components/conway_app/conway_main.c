#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

static inline int get_pixel(uint8_t *fb, int x, int y)
{
#ifdef DO_NOT_WRAP
    if ((x < 0) || (y > 0) || (x > PAINTER_SCREEN_WIDTH) || (y > PAINTER_SCREEN_HEIGHT)) {
        return 0;
    }
#else
    x = (x % PAINTER_SCREEN_WIDTH);
    y = (y % PAINTER_SCREEN_HEIGHT);
#endif

    int pixel_byte = ((y / 8) * WIDTH) + x;
    int pixel_bit = (y % 8);

    uint8_t block = 1 << pixel_bit;
    if (fb[pixel_byte] & block) {
        return PAINTER_BLACK;
    } else {
        return PAINTER_WHITE;
    }
}

void calculate_next_gen(void *fb, void *next_frame)
{
    for (int i = 0; i < PAINTER_SCREEN_WIDTH; i++) {
        for (int j = 0; j < PAINTER_SCREEN_HEIGHT; j++) {

            int num_adj = 0;
            int alive = 0;

            for (int off_x = -1; off_x <= 1; off_x++) {
                for (int off_y = -1; off_y <= 1; off_y++) {
                    if (get_pixel(fb, i + off_x, j + off_y) == PAINTER_BLACK) {
                        if ((off_x == 0) && (off_y == 0)) {
                            alive = 1;
                        } else {
                            num_adj++;
                        }
                    }
                }
            }

            int next_alive = PAINTER_WHITE;
            if (num_adj < 2) {
                next_alive = PAINTER_WHITE;

            } else if (alive && ((num_adj == 2) || (num_adj == 3))) {
                next_alive = PAINTER_BLACK;

            } else if (alive && (num_adj > 3)) {
                next_alive = PAINTER_WHITE;

            } else if (!alive && (num_adj == 3)) {
                next_alive = PAINTER_BLACK;
            }

            put_pixel(next_frame, i, j, next_alive);
        }
    }
}

void initialize_game(void *hwcontext)
{
    painter_clear_screen(hwcontext);
    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    for (int i = 0; i < PAINTER_SCREEN_WIDTH; i++) {
        for (int j = 0; j < PAINTER_SCREEN_HEIGHT; j++) {
            if (rand() % 10 == 0) {
                put_pixel(fb, i, j, PAINTER_BLACK);
            }
        }
    }

    hwcontext_update_screen(hwcontext);
}

void conway_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    char c;
    int pressed;
    struct timespec ts;

    uint8_t *fb = hwcontext_get_framebuffer(hwcontext);

    void *next_frame = malloc((WIDTH * HEIGHT) / 8);

    initialize_game(hwcontext);
    hwcontext_nb_get_key_code(hwcontext, &pressed, &ts, 1000);

    do {
        calculate_next_gen(fb, next_frame);
        memcpy(fb, next_frame, (WIDTH * HEIGHT) / 8);
        hwcontext_update_screen(hwcontext);

        c = hwcontext_nb_get_key_code(hwcontext, &pressed, &ts, 1000);

        if (c == '0') {
            initialize_game(hwcontext);
            hwcontext_nb_get_key_code(hwcontext, &pressed, &ts, 1000);
        }
    } while ((c != 'C') || pressed);
}
