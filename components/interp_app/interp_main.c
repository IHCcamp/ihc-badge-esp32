#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PUTCHAR(val) **outbuf = val; (*outbuf)++;
#define GETCHAR() 0

void execute_step(char **prgptr, char **memptr, char **outbuf)
{
    char *ptr = *memptr;
    char *pp = *prgptr;

    switch (*pp) {
        case '>':
            ++ptr;
            ++pp;
            break;

        case '<':
            --ptr;
            ++pp;
            break;

        case '+':
            ++*ptr;
            ++pp;
            break;

        case '-':
            --*ptr;
            ++pp;
            break;

        case '.':
            PUTCHAR(*ptr);
            ++pp;
            break;

        case ',':
            *ptr = GETCHAR();
            ++pp;
            break;

        case '[':
            if (*ptr == 0) {
                int level = 1;

                do {
                    ++pp;
                    if (*pp == ']') {
                        level--;

                    } else if (*pp == '[') {
                        level++;
                    }

                } while (level);
            } else {
                ++pp;
            }

            break;

        case ']':
            if (*ptr) {
                int level = 1;

                do {
                    --pp;
                    if (*pp == ']') {
                        level++;

                    } else if (*pp == '[') {
                        level--;
                    }
                } while (level);

            } else {
                ++pp;
            }

            break;

        default:
            abort();
    }

    *memptr = ptr;
    *prgptr = pp;
}

void interp_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    char outbuf[32];
    memset(outbuf, 0, 32);

    char memory[512];
    memset(memory, 0, 512);
    char *hello_world = "+[-[<<[+[--->]-[<<<]]]>>>-]>-.---.>..>.<<<<-.<+.>>>>>.>.<<.<-.";

    char *prg_ptr = hello_world;
    char *mem_ptr = memory;
    char *outbuf_ptr = outbuf;

    while (*prg_ptr) {
        execute_step(&prg_ptr, &mem_ptr, &outbuf_ptr);
    }
    ui_show_message(hwcontext, outbuf);
}
