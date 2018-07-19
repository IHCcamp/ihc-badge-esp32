#include "hwcontext.h"
#include "painter.h"

#include <stdio.h>

void shell_main(void *hwcontext)
{
    painter_draw_h_line(hwcontext, 2, 4, 30, PAINTER_BLACK);

    int c;
    do {
        c = hwcontext_get_key_code(hwcontext);
        printf("last char: %c\n", c);
    } while(c != 'q');
}
