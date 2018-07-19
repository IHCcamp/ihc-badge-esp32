#include "hwcontext.h"
#include "sdlhwcontext.h"

#include <stdint.h>
#include <unistd.h>

int hwcontext_get_key_code(void *hw_context)
{
    struct HWContext *hw = (struct HWContext *) hw_context;

    char c;
    read(hw->data_fd, &c, 1);

    return c;
}

uint8_t *hwcontext_get_framebuffer(void *hw_context)
{
    struct HWContext *hw = (struct HWContext *) hw_context;
    return hw->screen_buf;
}
