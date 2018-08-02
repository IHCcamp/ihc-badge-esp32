#ifndef _APPCONTEXT_H_
#define _APPCONTEXT_H_

#include "linkedlist.h"

struct AppContext
{
    void *hwcontext;
    char *user_name;
    struct ListHead *msgs;
    char *serial_number;
    char *phone_number;
};

#endif
