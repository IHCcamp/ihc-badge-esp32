#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shell_menu.h"
#include "startup.xbm"

void shell_main(void *hwcontext)
{
    int c;
    int pressed;
    struct timespec ts;

    ui_draw_animation(hwcontext, startup_frame_count, 5, startup, 0, 0, startup_width, startup_height);

    while (1) {
        painter_clear_screen(hwcontext);
        int menu_width = painter_painted_text_width("Menu", PAINTER_FONT_REGULAR);
        painter_draw_text(hwcontext, (PAINTER_SCREEN_WIDTH - menu_width) / 2, 5, "Menu", PAINTER_FONT_REGULAR, PAINTER_BLACK);
        hwcontext_update_screen(hwcontext);

        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);

        if (!pressed) {
            switch (c) {
                case 'M':
                    show_shell_menu(hwcontext);
                    break;
            }
        }
    }
}
