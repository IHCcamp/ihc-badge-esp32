#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void snake_main(struct AppContext *appctx);

#define CHOICES_ENTRIES 1

void games_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    const char *const choices[] = {
        "Snake"
    };

    int choice;
    do {
        choice = ui_show_menu(hwcontext, CHOICES_ENTRIES, choices, 0);
        switch(choice) {
            case 0:
                snake_main(appctx);
                break;
        }
    } while (choice >= 0);
}
