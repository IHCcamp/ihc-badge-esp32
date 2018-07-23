#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ihc_logo.xbm"
#include "startup.xbm"

void shell_main(void *hwcontext)
{
    int x_pos = 0;
    int y_pos = 0;

    int c;
    int pressed;
    struct timespec ts;

    const char *const choices[] = {
        "Booting",
        "Lisa Frank 420",
        "Flower Shoppe",
        "Library",
        "Geography",
        "Chill Divin' with ECCO",
        "Mathematics",
        "Foreign Banks Av...",
        "Te",
        "Moon",
        "Seabed"
    };

    ui_draw_animation(hwcontext, startup_frame_count, 5, startup, 0, 0, startup_width, startup_height);
    int choice = ui_show_menu(hwcontext, 11, choices, 2);
    ui_show_message(hwcontext, choices[choice]);
    ui_show_message(hwcontext, "Ready?");
    char *in = ui_ask_user_input(hwcontext, "Write stuff");
    if (in) {
        ui_show_message(hwcontext, in);
    } else {
        ui_show_message(hwcontext, "You're lazy");
    }
    free(in);

    for (int i = 10; i >= 0; i--) {
        char buf[3];
        sprintf(buf, "%i", i);
        painter_clear_screen(hwcontext);
        painter_draw_text(hwcontext, 40, 2, buf, PAINTER_FONT_REGULAR, PAINTER_BLACK);
        hwcontext_update_screen(hwcontext);
        hwcontext_delay_ms(hwcontext, 1000);
    }

    painter_clear_screen(hwcontext);
    painter_draw_text(hwcontext, 20, 2, "Hello 3310!", PAINTER_FONT_REGULAR, PAINTER_BLACK);
    hwcontext_update_screen(hwcontext);
    hwcontext_delay_ms(hwcontext, 1000);

    painter_clear_screen(hwcontext);
    painter_draw_rect(hwcontext, 5, 5, 74, 38, PAINTER_BLACK);
    hwcontext_update_screen(hwcontext);

    do {
        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);
        printf("last char: %c pressed: %d timestamp: secs %ld nsecs %ld\n", c, pressed, ts.tv_sec, ts.tv_nsec);

        painter_clear_screen(hwcontext);
        painter_draw_xbm(hwcontext, ihc_logo_bits, 0, 0, ihc_logo_width, ihc_logo_height);
        painter_draw_rect(hwcontext, 5, 5, 74, 38, PAINTER_BLACK);
        if (pressed) {
            switch (c) {
                case '6':
                    x_pos += 1;
                    break;
                case '4':
                    x_pos -= 1;
                    break;
                case '8':
                    y_pos += 1;
                    break;
                case '2':
                    y_pos -= 1;
                    break;
            }
        }
        painter_draw_rect(hwcontext, x_pos, y_pos, 3, 3, PAINTER_BLACK);
        hwcontext_update_screen(hwcontext);

    } while (c != 'C');
}
