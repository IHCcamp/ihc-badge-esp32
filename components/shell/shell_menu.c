#include "shell_menu.h"

#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "apps/apps_list.h"

static int count_menu_entries(const struct MenuEntry *entries)
{
    int i = 0;

    while (entries[i].title != NULL) {
        i++;
    }

    return i;
}

static void display_shell_menu(void *hwcontext, int current_entry)
{
    int title_width = painter_painted_text_width(menu_entries[current_entry].title, PAINTER_FONT_BOLD);

    painter_clear_screen(hwcontext);
    painter_draw_text(hwcontext, (PAINTER_SCREEN_WIDTH - title_width) / 2, 1, menu_entries[current_entry].title, PAINTER_FONT_BOLD, PAINTER_BLACK);
    painter_draw_xbm(hwcontext, menu_entries[current_entry].menu_pixmap, 10, 20, 64, 14);
    ui_print_menu_button_label(hwcontext, "Select");
    hwcontext_update_screen(hwcontext);
}

void show_shell_menu(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;
    int num_entries = count_menu_entries(menu_entries);
    int current_entry = 0;

    char c;
    int pressed;

    do {
        painter_clear_screen(hwcontext);
        display_shell_menu(hwcontext, current_entry);

        struct timespec ts;
        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);

        if (!pressed && (c == 'U') && (current_entry > 0)) {
            current_entry--;

        } else if (!pressed && (c == 'U')) {
            current_entry = num_entries - 1;

        } else if (!pressed && (c == 'D')) {
            current_entry = (current_entry + 1) % num_entries;
        }
    } while (((c != 'M') && ( c != 'C')) || pressed);

    if (c != 'C') {
        menu_entries[current_entry].app_main(appctx);
    }
}

