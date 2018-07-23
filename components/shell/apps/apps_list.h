#ifndef _APPS_LIST_H_
#define _APPS_LIST_H_

#include "appcontext.h"

typedef void (*app_main_t)(struct AppContext *appctx);

struct MenuEntry
{
    const char *title;
    const void *menu_pixmap;
    app_main_t app_main;
};

extern const struct MenuEntry menu_entries[];

#endif
