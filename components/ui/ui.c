#include "hwcontext.h"
#include "input.h"
#include "painter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void ui_print_menu_button_label(void *hwcontext, const char *label)
{
    int label_width = painter_painted_text_width(label, PAINTER_FONT_BOLD);
    painter_draw_text(hwcontext, (PAINTER_SCREEN_WIDTH - label_width) / 2, 5, label, PAINTER_FONT_BOLD, PAINTER_BLACK);
    hwcontext_update_screen(hwcontext);
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

char *ui_ask_user_input(void *hwcontext, const char *message)
{
    painter_clear_screen(hwcontext);
    painter_draw_text(hwcontext, 0, 0, message, PAINTER_FONT_REGULAR, PAINTER_BLACK);
    hwcontext_update_screen(hwcontext);
    char c;
    int pressed;
    struct timespec ts;
    struct EditingState es;
    input_init_editing_state(&es);
    do {
        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);
        input_consume_key_event(&es, c, pressed, &ts);
        painter_draw_fill_rect(hwcontext, 0, ROW_HEIGHT * 2, PAINTER_SCREEN_WIDTH, PAINTER_SCREEN_HEIGHT - ROW_HEIGHT * 2, PAINTER_WHITE);
        painter_draw_text(hwcontext, 0, 2, es.buffer, PAINTER_FONT_REGULAR, PAINTER_BLACK);
        hwcontext_update_screen(hwcontext);
    } while ((c != 'M') || (pressed != 0));

    if (es.last_char < 0) {
        return NULL;
    }

    // Buffer length is last_char + 1 + 1 for NULL terminator
    size_t length = es.last_char + 2;
    return strndup(es.buffer, length);
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

void ui_draw_animation(void *hwcontext, int framescount, int fps, const unsigned char *img_bits_frame, int x, int y, int width, int height)
{
    for(int i = 0; i < framescount; i++){
        painter_draw_xbm(hwcontext, img_bits_frame + (((width + 4) * height) / 8) * i,  x, y, width, height);
        hwcontext_update_screen(hwcontext);
        hwcontext_delay_ms(hwcontext, (1.0 / fps) * 1000);  
    }
}
