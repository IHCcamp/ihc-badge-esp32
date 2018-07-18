#include <stdio.h>
#include <pthread.h>
#include <SDL.h>
#include <unistd.h>

#include "utils.h"

#define WIDTH 84
#define HEIGHT 48

#define SDL_WIDTH (WIDTH * 8)
#define SDL_HEIGHT (HEIGHT * 8)
#define BPP 4
#define DEPTH 32

struct HWContext
{
    int data_fd;
    void *screen_buf;
};

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

    int pixel_index = 0;
    for (int y = 0; y < HEIGHT; y++ ) {
        for (int x = 0; x < WIDTH; x++ ) {
            int color;
            if (screen_buf[pixel_index / 8] & (1 << (pixel_index % 8))) {
                color = 0;
            } else {
                color = 255;
            }
            for (int y_dest = y * 8; y_dest < (y + 1) * 8; y_dest++) {
                for (int x_dest = x * 8; x_dest < (x + 1) * 8; x_dest++) {
                  sdl_setpixel(screen, x_dest, y_dest, color, color, color);
                }
            }
            pixel_index++;
        }
    }

    if(SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }

    SDL_Flip(screen);
}

void *ui_loop(void *arg)
{
    struct HWContext *hw = (struct HWContext *) arg;

    char c;

    do {
        memset(hw->screen_buf, 0, (WIDTH * HEIGHT / 8));
        read(hw->data_fd, &c, 1);
    } while(c != 'q');

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

    int fds[2];
    pipe(fds);

    uint8_t *screen_buf = malloc((WIDTH * HEIGHT) / 8);
    struct HWContext *hw = malloc(sizeof(struct HWContext));
    hw->screen_buf = screen_buf;
    hw->data_fd = fds[0];

    int write_fd = fds[1];

    pthread_t thread_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&thread_id, &attr, ui_loop, hw);

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
