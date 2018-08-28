#include "appcontext.h"
#include "hwcontext.h"
#include "ui.h"

void midi_ble_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    ui_show_message(hwcontext, "Not available on emulator");
}
