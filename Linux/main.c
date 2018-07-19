#include <stdio.h>
#include <pthread.h>
#include <SDL.h>
#include <unistd.h>

#include "sdlhwcontext.h"
#include "utils.h"

#define SCREEN_WIDTH 84
#define SCREEN_HEIGHT 48

#define WIDTH (84 + 4)
#define HEIGHT 48

#define SDL_WIDTH (WIDTH * 8)
#define SDL_HEIGHT (HEIGHT * 8)
#define BPP 4
#define DEPTH 32

void shell_main(void *hwcontext);

void sdl_setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;

    pixmem32 = (Uint32*) (((uint8_t *) screen->pixels) + (screen->w * y + x) * BPP);
    *pixmem32 = SDL_MapRGB(screen->format, r, g, b);
}

void sdl_draw_screen(SDL_Surface* screen, uint8_t *screen_buf)
{
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            return;
        }
    }

    for (int y = 0; y < SCREEN_HEIGHT; y++ ) {
        for (int x = 0; x < SCREEN_WIDTH; x++ ) {
            int color;

            int pixel_byte = ((y / 8) * WIDTH) + x;
            int pixel_bit = (y % 8);

            if (screen_buf[pixel_byte] & (1 << pixel_bit)) {
                color = 0;
            } else {
                color = 255;
            }
            for (int y_dest = y * 8; y_dest < (y + 1) * 8; y_dest++) {
                for (int x_dest = x * 8; x_dest < (x + 1) * 8; x_dest++) {
                  sdl_setpixel(screen, x_dest, y_dest, color, color, color);
                }
            }
        }
    }

    if(SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }

    SDL_Flip(screen);
}

void *ui_start(void *arg)
{
    shell_main(arg);

    return NULL;
}

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    SDL_Surface *screen;
    SDL_Event event;

    int quit = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        return EXIT_FAILURE;
    }

    if (!(screen = SDL_SetVideoMode(SDL_WIDTH, SDL_HEIGHT, DEPTH, SDL_HWSURFACE)))
    {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    memset(screen->pixels, 0x80, SDL_WIDTH * SDL_HEIGHT * BPP);

    int fds[2];
    pipe(fds);

    uint8_t *screen_buf = malloc((WIDTH * HEIGHT) / 8);
    struct HWContext *hw = malloc(sizeof(struct HWContext));
    hw->screen_buf = screen_buf;
    memset(hw->screen_buf, 0, (WIDTH * HEIGHT / 8));
    hw->data_fd = fds[0];

    int write_fd = fds[1];

    pthread_t thread_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&thread_id, &attr, ui_start, hw);

    while(!quit) {

        sdl_draw_screen(screen, screen_buf);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
	            quit = 1;
                    int quit_c = 'q';
                    write(write_fd, &quit_c, 1);
	            break;

                default:
                    break;
            }
        }
        usleep(33333);
    }

    SDL_Quit();

    void *res;
    pthread_join(thread_id, &res);

    return EXIT_SUCCESS;
}
