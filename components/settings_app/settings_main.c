#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CHOICES_ENTRIES 1

#define SET_NAME_ENTRY 0

static void set_name(struct AppContext *appctx);

void settings_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    const char *const choices[] = {
        "Set name"
    };

    int choice;
    do {
        choice = ui_show_menu(hwcontext, CHOICES_ENTRIES, choices, 0);
        switch(choice) {
            case SET_NAME_ENTRY:
                set_name(appctx);
                break;
        }
    } while (choice >= 0);
}

static void set_name(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    char *new_name = ui_ask_user_input(hwcontext, "Insert name");
    if (new_name) {
        free(appctx->user_name);
        appctx->user_name = new_name;
        hwcontext_set_nv_string(hwcontext, "user_name", new_name);
    }
}
