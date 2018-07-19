#include "hwcontext.h"
#include "painter.h"

#include <stdio.h>

void shell_main(void *hwcontext)
{
    painter_draw_rect(hwcontext, 5, 5, 74, 38, PAINTER_BLACK);
    hwcontext_update_screen(hwcontext);

    int c;
    int pressed;
    struct timespec ts;
    do {
        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);
        printf("last char: %c pressed: %d timestamp: secs %ld nsecs %ld\n", c, pressed, ts.tv_sec, ts.tv_nsec);
    } while (c != 'C');
}
