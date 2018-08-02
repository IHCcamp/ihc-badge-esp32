#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void phonebook_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    char buf[32];
    sprintf(buf, "Your phone number is: %s.\n", appctx->phone_number);

    ui_show_message(hwcontext, buf);
}
