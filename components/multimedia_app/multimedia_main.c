#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CHOICES_ENTRIES 1

void midi_ble_main(struct AppContext *appctx);

void multimedia_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    const char *const choices[] = {
        "BLE MIDI Ctrl",
    };

    int choice;
    do {
        choice = ui_show_menu(hwcontext, CHOICES_ENTRIES, choices, 0);
        switch(choice) {
            case 0:
                midi_ble_main(appctx);
                break;
        }
    } while (choice >= 0);
}
