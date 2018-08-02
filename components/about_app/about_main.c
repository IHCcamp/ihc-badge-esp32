#include "appcontext.h"
#include "hwcontext.h"
#include "commands.h"
#include "painter.h"
#include "ui.h"

#include "ph.xbm"
#include "o1.xbm"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//ringtone lasts 10 seconds
#define A_delay 2500

void CenterText(void *hwcontext, const char *text, int row);

void about_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    hwcontext_send_command(hwcontext, RINGTONE_CMD, "8");

    painter_draw_xbm(hwcontext, ph_bits, 0, 0, ph_width, ph_height);
    CenterText(hwcontext, "Idea and HW",0);
    CenterText(hwcontext, "design",1);
    CenterText(hwcontext, "Mastro Gippo",3);
    hwcontext_update_screen(hwcontext);
    hwcontext_delay_ms(hwcontext, A_delay);
    ui_draw_animation(hwcontext, o1_frame_count, 5, o1_bits, 0, 0, o1_width, o1_height);
    
    painter_draw_xbm(hwcontext, ph_bits, 0, 0, ph_width, ph_height);
    CenterText(hwcontext, "ESP32 Firmware",0);
    CenterText(hwcontext, "Uninstall", 3);
    hwcontext_update_screen(hwcontext);
    hwcontext_delay_ms(hwcontext, A_delay);
    ui_draw_animation(hwcontext, o1_frame_count, 5, o1_bits, 0, 0, o1_width, o1_height);

    painter_draw_xbm(hwcontext, ph_bits, 0, 0, ph_width, ph_height);
    CenterText(hwcontext, "ESP32 Firmware",0);
    CenterText(hwcontext, "rbino",3);
    hwcontext_update_screen(hwcontext);
    hwcontext_delay_ms(hwcontext, A_delay);
    ui_draw_animation(hwcontext, o1_frame_count, 5, o1_bits, 0, 0, o1_width, o1_height);
}

void CenterText(void *hwcontext, const char *text, int row)
{
    
    int x_pos = 0;

    int x_bound = PAINTER_SCREEN_WIDTH;
    int str_width = painter_painted_text_width(text, PAINTER_FONT_BOLD);

    if (str_width < x_bound - x_pos) {
        x_pos = (PAINTER_SCREEN_WIDTH - str_width) / 2;
    }

    painter_draw_bounded_text(hwcontext,
            x_pos, //x
            row, // row
            x_bound, // bound x
            row+1, // bound row
            text, PAINTER_FONT_BOLD, PAINTER_BLACK
        );
}
