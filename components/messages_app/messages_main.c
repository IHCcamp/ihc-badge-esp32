#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void messages_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    printf("Henlo World\n");
}
