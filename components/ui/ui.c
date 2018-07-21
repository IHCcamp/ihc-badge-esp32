#include "hwcontext.h"
#include "painter.h"

#include <stdio.h>
#include <time.h>

static void wait_menu_key_press(void *hwcontext);

void ui_show_message(void *hwcontext, const char *message)
{
    int ok_width = painter_painted_text_width("Ok", PAINTER_FONT_REGULAR);

    painter_clear_screen(hwcontext);
    painter_draw_text(hwcontext, 0, 0, message, PAINTER_FONT_REGULAR);
    painter_draw_text(hwcontext, (PAINTER_SCREEN_WIDTH - ok_width) / 2, 5, "Ok", PAINTER_FONT_REGULAR);
    hwcontext_update_screen(hwcontext);

    wait_menu_key_press(hwcontext);
}

static void wait_menu_key_press(void *hwcontext)
{
    char c;
    int pressed;
    do {
        struct timespec ts;
        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);
    } while ((c != 'M') || (pressed != 0));
}
