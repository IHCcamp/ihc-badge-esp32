#include "hwcontext.h"
#include "esp32hwcontext.h"

uint8_t *hwcontext_get_framebuffer(void *hw_context)
{
    struct HWContext *hw = (struct HWContext *) hw_context;
    return u8g2_GetBufferPtr(hw->u8g2);
}

void hwcontext_update_screen(void *hw_context)
{
    struct HWContext *hw = (struct HWContext *) hw_context;
    u8g2_SendBuffer(hw->u8g2);
}
