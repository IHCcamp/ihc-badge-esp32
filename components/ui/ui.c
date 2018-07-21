#include "hwcontext.h"
#include "painter.h"

#include <stdio.h>
#include <time.h>

#define ROW_HEIGHT 8

static void display_menu(void *hwcontext, int num_entries, const char * const *entries, int current_entry);
static void wait_menu_key_press(void *hwcontext);

void ui_show_message(void *hwcontext, const char *message)
{
    int ok_width = painter_painted_text_width("Ok", PAINTER_FONT_REGULAR);

    painter_clear_screen(hwcontext);
    painter_draw_text(hwcontext, 0, 0, message, PAINTER_FONT_REGULAR, PAINTER_BLACK);
    painter_draw_text(hwcontext, (PAINTER_SCREEN_WIDTH - ok_width) / 2, 5, "Ok", PAINTER_FONT_REGULAR, PAINTER_BLACK);
    hwcontext_update_screen(hwcontext);

    wait_menu_key_press(hwcontext);
}

int ui_show_menu(void *hwcontext, int num_entries, const char * const *entries, int current_entry)
{
    char c;
    int pressed;

    do {
        painter_clear_screen(hwcontext);
        display_menu(hwcontext, num_entries, entries, current_entry);

        struct timespec ts;
        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);

        if (!pressed && (c == 'U') && (current_entry > 0)) {
            current_entry--;
        } else if (!pressed && (c == 'D') && (current_entry < num_entries - 1)) {
            current_entry++;
        }
    } while ((c != 'M') || pressed);

    return current_entry;
}

static void display_menu(void *hwcontext, int num_entries, const char * const *entries, int current_entry)
{
    int first_visible_entry = (current_entry / 6) * 6;
    int current_visible_entry = current_entry % 6;
    int visible_rows = (num_entries - first_visible_entry > 6) ? 6 : num_entries - first_visible_entry;

    for (int i = 0; i < visible_rows; i++) {
        int color = PAINTER_BLACK;

        if (i == current_visible_entry) {
            painter_draw_fill_rect(hwcontext, 0, i * ROW_HEIGHT, PAINTER_SCREEN_WIDTH, ROW_HEIGHT - 1, PAINTER_BLACK);
            color = PAINTER_WHITE;
        }

        painter_draw_text(hwcontext, 1, i, entries[first_visible_entry + i], PAINTER_FONT_REGULAR, color);
    }
    hwcontext_update_screen(hwcontext);
}

static void wait_menu_key_press(void *hwcontext)
{
    char c;
    int pressed;
    do {
        struct timespec ts;
        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);
    } while ((c != 'M') || pressed);
}
