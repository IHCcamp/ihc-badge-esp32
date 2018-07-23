#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void settings_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    const char *const choices[] = {
        "Booting",
        "Lisa Frank 420",
        "Flower Shoppe",
        "Library",
        "Geography",
        "Chill Divin' with ECCO",
        "Mathematics",
        "Foreign Banks Av...",
        "Te",
        "Moon",
        "Seabed"
    };

    ui_show_menu(hwcontext, 11, choices, 2);
}
