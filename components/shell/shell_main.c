#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shell_menu.h"
#include "battery_icon.xbm"
#include "signal_icon.xbm"
#include "startup.xbm"
#include "logo.xbm"

static const uint8_t strength_icon_heights[] = {
    6,
    7,
    7,
    8
};

static const uint8_t strength_icon_widths[] = {
    2,
    2,
    3,
    4
};

static void draw_signal_status(void *hwcontext);
static void draw_battery_status(void *hwcontext);
static void draw_name(void *hwcontext, const char *name);

void shell_main(void *hwcontext)
{
    int c;
    int pressed;
    struct timespec ts;
    struct AppContext appctx;
    appctx.hwcontext = hwcontext;
    appctx.user_name = hwcontext_get_nv_string(hwcontext, "user_name", "IHC 2018");
    appctx.msgs = NULL;

    ui_draw_animation(hwcontext, startup_frame_count, 5, startup, 0, 0, startup_width, startup_height);
    painter_clear_screen(hwcontext);
    painter_draw_xbm(hwcontext, logo_bits, 0, 0, logo_width, logo_height);
    hwcontext_update_screen(hwcontext);
    hwcontext_delay_ms(hwcontext, 1000);

    while (1) {
        painter_clear_screen(hwcontext);
        ui_print_menu_button_label(hwcontext, "Menu");
        draw_signal_status(hwcontext);
        draw_battery_status(hwcontext);
        draw_name(hwcontext, appctx.user_name);

        hwcontext_update_screen(hwcontext);

        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);

        if (!pressed) {
            switch (c) {
                case 'M':
                    show_shell_menu(&appctx);
                    break;
            }
        }
    }
}

static void draw_signal_status(void *hwcontext)
{
    uint8_t signal_strength = 100;

    int y = 32;

    painter_draw_xbm(hwcontext, signal_icon_bits, 0, y, signal_icon_width, signal_icon_height);

    y -= signal_icon_height + 1;

    for (size_t i = 0; i < sizeof(strength_icon_heights) && signal_strength > i * 25; i++) {
        int height = strength_icon_heights[i];
        int width = strength_icon_widths[i];
        painter_draw_fill_rect(hwcontext, 0, y, width, height, PAINTER_BLACK);
        if (i + 1 <= sizeof(strength_icon_widths)) {
            y -= strength_icon_heights[i + 1] + 1;
        }
    }
}

static void draw_battery_status(void *hwcontext)
{
    uint8_t battery_charge = 100;

    int y = 32;
    int x = PAINTER_SCREEN_WIDTH - battery_icon_width;

    painter_draw_xbm(hwcontext, battery_icon_bits, x, y, battery_icon_width, battery_icon_height);

    y -= battery_icon_height + 1;

    for (size_t i = 0; i < sizeof(strength_icon_heights) && battery_charge > i * 25; i++) {
        int height = strength_icon_heights[i];
        int width = strength_icon_widths[i];
        int x = PAINTER_SCREEN_WIDTH - width;
        painter_draw_fill_rect(hwcontext, x, y, width, height, PAINTER_BLACK);
        if (i + 1 <= sizeof(strength_icon_widths)) {
            y -= strength_icon_heights[i + 1] + 1;
        }
    }
}

static void draw_name(void *hwcontext, const char *name)
{
    int x_pos = signal_icon_width + 2;

    int x_bound = PAINTER_SCREEN_WIDTH - battery_icon_width - 2;
    int name_width = painter_painted_text_width(name, PAINTER_FONT_BOLD);

    if (name_width < x_bound - x_pos) {
        x_pos = (PAINTER_SCREEN_WIDTH - name_width) / 2;
    }

    painter_draw_bounded_text(hwcontext,
            x_pos, //x
            2, // row
            x_bound, // bound x
            5, // bound row
            name, PAINTER_FONT_BOLD, PAINTER_BLACK
        );
}
