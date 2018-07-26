#ifndef _ESP32HWCONTEXT_
#define _ESP32HWCONTEXT_

#include <u8g2.h>

struct HWContext
{
    u8g2_t *u8g2;
    void *key_events_queue;
};

#endif
