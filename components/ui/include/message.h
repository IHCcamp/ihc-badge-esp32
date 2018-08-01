#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "linkedlist.h"
#include <time.h>

struct Message
{
    struct ListHead message_list_head;
    const char *topic;
    const char *data;
    struct timespec timestamp;
    int data_len;
};

#endif
