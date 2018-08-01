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

    const char *hello_world = "+[-[<<[+[--->]-[<<<]]]>>>-]>-.---.>..>.<<<<-.<+.>>>>>.>.<<.<-.";
    char program[512];
    memset(program, 0, sizeof(program));
    memcpy(program, hello_world, strlen(hello_world));

    painter_clear_screen(hwcontext);
    painter_draw_text(hwcontext, 0, 0, program, PAINTER_FONT_REGULAR, PAINTER_BLACK);

    char c;
    int pressed;
    unsigned int program_i = strlen(program);
    int run = 0;

    do {
        struct timespec ts;
        c = hwcontext_get_key_code(hwcontext, &pressed, &ts);

        if (!pressed) {
            switch (c) {
                case '0':
                    program[program_i] = '>';
                    program_i++;
                    break;

                case '1':
                    program[program_i] = '<';
                    program_i++;
                    break;

                case '2':
                    program[program_i] = '+';
                    program_i++;
                    break;

                case '3':
                    program[program_i] = '-';
                    program_i++;
                    break;

                case '4':
                    program[program_i] = '.';
                    program_i++;
                    break;

                case '5':
                    program[program_i] = ',';
                    program_i++;
                    break;

                case '6':
                    program[program_i] = '[';
                    program_i++;
                    break;

                case '7':
                    program[program_i] = ']';
                    program_i++;
                    break;

                case '8':
                    program_i--;
                    program[program_i] = '\0';
                    break;

                case '9':
                    memset(program, 0, sizeof(program));
                    program_i = 0;
                    break;

                case 'U':
                    if (program_i > 0) {
                        program_i--;
                    }
                    break;

                case 'D':
                    if (program_i < sizeof(program)) {
                        program_i++;
                    }
                    break;

                case 'M':
                    run = 1;
                    break;

                case 'C':
                    return;
            }

            painter_clear_screen(hwcontext);
            painter_draw_text(hwcontext, 0, 0, program, PAINTER_FONT_REGULAR, PAINTER_BLACK);
        }
    } while (!run);


    char *prg_ptr = program;
    char *mem_ptr = memory;
    char *outbuf_ptr = outbuf;

    while (*prg_ptr) {
        execute_step(&prg_ptr, &mem_ptr, &outbuf_ptr);
    }
    ui_show_message(hwcontext, outbuf);
}
