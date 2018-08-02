#ifndef _HWCONTEXT_H_
#define _HWCONTEXT_H_

#include <stdint.h>
#include <time.h>

char hwcontext_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp);
int hwcontext_nb_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp, int timeout);
uint8_t *hwcontext_get_framebuffer(void *hw_context);
void hwcontext_update_screen(void *hw_context);
void hwcontext_delay_ms(void *hw_context, int mseconds);
int hwcontext_set_nv_string(void *hwcontext, const char *key, const char *value);
char *hwcontext_get_nv_string(void *hwcontext, const char *key, const char *default_value);

void hwcontext_send_command(void *hwcontext, const char *command, const char *command_args);

#endif
