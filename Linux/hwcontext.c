#include "hwcontext.h"
#include "sdlhwcontext.h"
#include "utils.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

char hwcontext_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp)
{
    struct HWContext *hw = (struct HWContext *) hwcontext;

    char c;
    read(hw->data_fd, &c, 1);
    *pressed = !(c & 0x80);

    clock_gettime(CLOCK_MONOTONIC, timestamp);

    fprintf(stderr, "key: %c, pressed: %i\n", c, *pressed);

    return toupper(c & 0x7F);
}

int hwcontext_nb_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp, int timeoutms)
{
    struct HWContext *hw = (struct HWContext *) hwcontext;

    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(hw->data_fd, &set);

    timeout.tv_sec = timeoutms / 1000;
    timeout.tv_usec = timeoutms % 1000;

    if (!select(FD_SETSIZE, &set, NULL, NULL, &timeout)) {
        return -1;
    }

    return hwcontext_get_key_code(hwcontext, pressed, timestamp);
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

void hwcontext_delay_ms(void *hw_context, int mseconds){
	UNUSED(hw_context);
	usleep(mseconds * 1000);
}

int hwcontext_set_nv_string(void *hwcontext, const char *key, const char *value)
{
    UNUSED(hwcontext);
    UNUSED(key);
    UNUSED(value);
    return -1;
}

char *hwcontext_get_nv_string(void *hwcontext, const char *key, const char *default_value)
{
    UNUSED(hwcontext);
    UNUSED(key);
    UNUSED(default_value);
    return NULL;
}
