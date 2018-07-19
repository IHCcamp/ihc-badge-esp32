#ifndef _HWCONTEXT_H_
#define _HWCONTEXT_H_

#include <stdint.h>

int hwcontext_get_key_code(void *hw_context);
uint8_t *hwcontext_get_framebuffer(void *hw_context);
void hwcontext_update_screen(void *hw_context);

#endif
