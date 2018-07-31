#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "linkedlist.h"

struct Message
{
    struct ListHead messages_list_head;
    const char *topic;
    const char *data;
    struct timespec timestamp;
    int data_len;
};

#endif
