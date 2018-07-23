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

void shell_main(void *hwcontext)
{
    int c;
    int pressed;
    struct timespec ts;

    ui_draw_animation(hwcontext, startup_frame_count, 5, startup, 0, 0, startup_width, startup_height);

    while (1) {
        painter_clear_screen(hwcontext);
        ui_print_menu_button_label(hwcontext, "Menu");
        draw_signal_status(hwcontext);
        draw_battery_status(hwcontext);

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
