#ifndef _HWCONTEXT_H_
#define _HWCONTEXT_H_

#include <stdint.h>
#include <time.h>

char hwcontext_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp);
uint8_t *hwcontext_get_framebuffer(void *hw_context);
void hwcontext_update_screen(void *hw_context);
void hwcontext_delay_ms(void *hw_context, int mseconds);

#endif
