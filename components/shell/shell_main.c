#include "hwcontext.h"

#include <stdio.h>

void shell_main(void *hwcontext)
{
    int c;
    do {
        c = hwcontext_get_key_code(hwcontext);
        printf("last char: %c\n", c);
    } while(c != 'q');
}
