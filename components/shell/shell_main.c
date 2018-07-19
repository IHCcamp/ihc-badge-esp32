#include "hwcontext.h"
#include "painter.h"

#include <stdio.h>

void shell_main(void *hwcontext)
{
    painter_draw_rect(hwcontext, 5, 5, 74, 38, PAINTER_BLACK);
    hwcontext_update_screen(hwcontext);

    int c;
    do {
        c = hwcontext_get_key_code(hwcontext);
        printf("last char: %c\n", c);
    } while(c != 'q');
}
