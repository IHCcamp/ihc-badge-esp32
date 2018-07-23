#include "apps_list.h"

#include <stdlib.h>

#include "app_entry_points.h"

#include "settings.xbm"

const struct MenuEntry menu_entries[] =
{
    {"Settings", settings_bits, settings_main},
    {NULL, NULL, NULL}
};

