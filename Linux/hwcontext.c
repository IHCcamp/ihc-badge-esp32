#include "hwcontext.h"
#include "sdlhwcontext.h"
#include "utils.h"

#include <stdint.h>
#include <time.h>
#include <unistd.h>

char hwcontext_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp)
{
    struct HWContext *hw = (struct HWContext *) hwcontext;

    char c;
    read(hw->data_fd, &c, 1);
    *pressed = 0;

    clock_gettime(CLOCK_MONOTONIC, timestamp);

    return c;
}

uint8_t *hwcontext_get_framebuffer(void *hw_context)
{
    struct HWContext *hw = (struct HWContext *) hw_context;
    return hw->screen_buf;
}

void hwcontext_update_screen(void *hw_context)
{
    UNUSED(hw_context);
}
