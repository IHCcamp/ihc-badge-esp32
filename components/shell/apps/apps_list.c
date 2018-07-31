#include "apps_list.h"

#include <stdlib.h>

#include "app_entry_points.h"

#include "settings.xbm"
#include "interp.xbm"
#include "games.xbm"

const struct MenuEntry menu_entries[] =
{
    {"Settings", settings_bits, settings_main},
    {"Interp.", interp_bits, interp_main},
    {"Games", games_bits, games_main},
    {NULL, NULL, NULL}
};

