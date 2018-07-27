#include "appcontext.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "splash.xbm"

#define WIDTH PAINTER_SCREEN_WIDTH
#define HEIGHT PAINTER_SCREEN_HEIGHT

enum {
    KEY_U = 1,
    KEY_D = 2,
    KEY_L = 4,
    KEY_R = 8,
    KEY_M = 16,
    KEY_C = 32,
};

static void *hwcontext;
static volatile int gKeys = 0;
static volatile int stop_event = 0;

static inline void put_pixel(int x, int y, int v)
{ 
    painter_draw_pixel(hwcontext, x, y, v & 1);
}

void get_keys()
{
    struct timespec timestamp;
    int pressed = -1;
    int keycode = hwcontext_nb_get_key_code(hwcontext, &pressed, &timestamp, 0);
    pressed = ((int8_t)(pressed<<7) >> 7);
    switch(keycode) {
        case 'W': case '2': gKeys = (gKeys & ~KEY_U) | (pressed & KEY_U); break;
        case 'A': case '4': gKeys = (gKeys & ~KEY_L) | (pressed & KEY_L); break;
        case 'S': case '8': gKeys = (gKeys & ~KEY_D) | (pressed & KEY_D); break;
        case 'D': case '6': gKeys = (gKeys & ~KEY_R) | (pressed & KEY_R); break;
        case 'M': gKeys = (gKeys & ~KEY_M) | (pressed & KEY_M); break;
        case 'Q': case 'C': stop_event = 1; break;
    }
}

//////////////// BEGIN GAME RELATED ////////////////

#define GWIDTH 17
#define GHEIGHT 7
#define SN_SX 5
#define SN_SY 5
#define SN_QS 256

#define BORDERY 8

enum {
    SCREEN_INIT = 0,
    SCREEN_GAME = 1,
    SCREEN_LOST = 2,
    SCREEN_SPLASH = 3,
    SCREEN_MENU = 4,
};

enum {
    GAREA_EMPTY = 0,
    GAREA_SNAKE = 1,
    GAREA_FOOD  = 2,
};

enum {
    GAREA_SNAKH = 0,
    GAREA_SNAKV = 1,
    GAREA_SNKLU = 2,
    GAREA_SNKRU = 3,
    GAREA_SNKLD = 4,
    GAREA_SNKRD = 5,
    GAREA_SNKHL = 6,
    GAREA_SNKHU = 7,
    GAREA_SNKHR = 8,
    GAREA_SNKHD = 9,
};

static inline uint8_t to_snp(uint8_t x, uint8_t y) 
{ 
    return y + (x << 3);
}

static inline void from_snp(uint8_t k, uint8_t* x, uint8_t* y) 
{  
    *x = k >> 3;
    *y = k & 0x07;
}

uint8_t game_area[GWIDTH * GHEIGHT];

static int16_t snake_queue[SN_QS] = {0};
static uint8_t game_mode = SCREEN_SPLASH;
static int16_t sn_hpos = 0, sn_tpos = 0;
static int8_t snake_dir = 0, snake_ndir = 0;
static int8_t delay = 0, max_delay = 0;
static int8_t food_active = 0;
static uint16_t snake_score = 0;
static int8_t dir_has_food = 0;

enum {
    TEX_SNAKH = 0,
    TEX_SNAKV = 1,
    TEX_SNKLU = 2,
    TEX_SNKRU = 3,
    TEX_SNKLD = 4,
    TEX_SNKRD = 5,
    TEX_SNKHL = 6,
    TEX_SNKHU = 7,
    TEX_SNKHR = 8,
    TEX_SNKHD = 9,
    TEX_SNOHL = 10,
    TEX_SNOHU = 11,
    TEX_SNOHR = 12,
    TEX_SNOHD = 13,
    TEX_FOOD1 = 14,
    TEX_FOOD2 = 15,
    TEX_FOOD3 = 16,

    __MAX_TEX__
};

#include "tiles.h"

static inline uint8_t sn_msq_body(uint8_t o, uint8_t n)
{
    static int8_t odx=1, ody=0;
#define MSQ(a,b,c,d,r) if(dx == a && dy == b && odx == c && ody == d) ret = r
    uint8_t ret = TEX_FOOD1;
    uint8_t ox, oy, nx, ny;
    from_snp(o, &ox, &oy);
    from_snp(n, &nx, &ny);
    int8_t dx = nx - ox, dy = ny - oy;

    /* Wrap around walls */
    if(dx < -1) dx += GWIDTH - 1;
    else if(dx > 1) dx -= (GWIDTH - 1);
    if(dy < -1) dy += GHEIGHT;
    else if(dy > 1) dy -= GHEIGHT;

    if(odx == dx && ody == dy) {
        if(dx == 1 || dx == -1)
            ret = GAREA_SNAKH;
        if(dy == 1 || dy == -1)
            ret = GAREA_SNAKV;
    } else {
        MSQ(-1 , 0 , 0 , 1, GAREA_SNKLU);
        MSQ( 1 , 0 , 0 ,-1, GAREA_SNKRD);
        MSQ( 0 ,-1 ,-1 , 0, GAREA_SNKRU);
        MSQ( 1 , 0 , 0 , 1, GAREA_SNKRU);
        MSQ( 0 ,-1 , 1 , 0, GAREA_SNKLU);
        MSQ(-1 , 0 , 0 ,-1, GAREA_SNKLD);
        MSQ( 0 , 1 , 1 , 0, GAREA_SNKLD);
        MSQ( 0 , 1 ,-1 , 0, GAREA_SNKRD);
    }

    odx = dx, ody = dy;
    return ret;
#undef MSQ
}

static inline void draw_tex(int x, int y, int w)
{
    for(int i=0;i<SN_SY;i++) {
        uint8_t b = tiles[w][i];
        for(int j=0;j<SN_SX;j++) {
            if((b >> ((SN_SX-1)-j)) & 1)
                put_pixel(2 + x * SN_SX + j, BORDERY + 2 + y * SN_SY + i, 1);
        }
    }
}

void draw_num(uint8_t n, uint8_t x, uint8_t y)
{
    if(n >= 10) {
        return;
    }

    for(uint8_t i=0;i<3;i++) {
        for(uint8_t j=0;j<5;j++) {
            put_pixel(x+i,y+j,(fonts_data[(n << 2)+i] >> j) & 1);
        }
    }
}

void draw_mnum(uint16_t n, uint8_t ndig, uint8_t x, uint8_t y)
{
    for(int i=0;i<ndig;i++) {
        uint16_t n2 = n % 10;
        draw_num(n2, x + (ndig - i - 1) * 4, y);
        n /= 10;
    }
}


void game_draw()
{
    draw_mnum(snake_score, 4, 2, 1);
    for(int i=0;i<WIDTH;i++) {
        put_pixel(i, BORDERY, 1);
        put_pixel(i, HEIGHT-1, 1);
    }

    for(int i=BORDERY;i<HEIGHT;i++) {
        put_pixel(0, i, 1);
        put_pixel(WIDTH-1, i, 1);
    }

    for(int y=0;y<GHEIGHT;y++) {
        for(int x=0;x<GWIDTH;x++) {
            uint8_t el = game_area[to_snp(x,y)];
            if((el & 0x0f) == GAREA_FOOD) {
                draw_tex(x,y,TEX_FOOD1 + (el >> 4));    
            }
            if((el & 0x0f) == GAREA_SNAKE) {
                uint8_t is_head = ((int8_t)(((el >> 4) >= GAREA_SNKHL) <<7))>>7;
                draw_tex(x,y,(el >> 4) + (is_head & (dir_has_food * 4)));
            }
        }
    }
}

void put_food()
{
    food_active = 1;
    for(int i=0;i<10;i++) {
        int x = rand() % GWIDTH, y = rand() % GHEIGHT;
        uint8_t pos = to_snp(x,y);
        if(game_area[pos] == GAREA_EMPTY) {
            game_area[pos] = GAREA_FOOD;
            food_active = 1;
            return;
        }
    }
}

void game_init()
{
    sn_hpos = 3;
    memset(snake_queue, 255, sizeof(snake_queue));
    memset(game_area, GAREA_EMPTY, sizeof(game_area));

    snake_queue[sn_hpos] = to_snp(GWIDTH/2, GHEIGHT/2);
    snake_dir = KEY_R;
    snake_ndir = snake_dir;
    snake_score = 0;
    delay = 0; //reset
    max_delay = 5;

    game_area[snake_queue[sn_hpos]] = GAREA_SNAKE | (GAREA_SNKHR << 4);

    uint8_t x, y;
    from_snp(snake_queue[sn_hpos], &x, &y);
    for(int i = 0; i < sn_hpos; i++) {
        int n = to_snp(x-i-1,y);
        snake_queue[sn_hpos-i-1] = n;
        game_area[n] = GAREA_SNAKE | (GAREA_SNAKH << 4);
    }

    put_food();

    /* init ok, start game loop */
    game_mode = SCREEN_GAME; 
}

void game_update()
{
    uint8_t nx, ny;
    uint8_t oldN = snake_queue[sn_hpos], newN;
    uint8_t sn_head_type = 0;
    from_snp(oldN, &nx, &ny);

    snake_dir = snake_ndir;

    switch(snake_dir) {
        case KEY_R: nx += 1; sn_head_type = GAREA_SNKHR; break;
        case KEY_L: nx -= 1; sn_head_type = GAREA_SNKHL; break;
        case KEY_U: ny -= 1; sn_head_type = GAREA_SNKHU; break;
        case KEY_D: ny += 1; sn_head_type = GAREA_SNKHD; break;
    }
    
    if(nx == 0xff) nx = GWIDTH - 2;
    if(ny == 0xff) ny = GHEIGHT - 1;
    if(nx == GWIDTH-1) nx = 0;
    if(ny == GHEIGHT)  ny = 0;

#define WRX(X,Y) ((game_area[to_snp(X,Y)]) & 0x0f)
    dir_has_food = 0;
    switch(snake_dir) {
        case KEY_R: 
            dir_has_food = WRX((nx+1)%(GWIDTH-1),ny) == GAREA_FOOD;
            break;
        case KEY_L: 
            dir_has_food = WRX((nx-1+GWIDTH-1)%(GWIDTH-1),ny) == GAREA_FOOD;
            break;
        case KEY_U: 
            dir_has_food = WRX(nx,(ny-1+GHEIGHT) % GHEIGHT) == GAREA_FOOD;
            break;
        case KEY_D: 
            dir_has_food = WRX(nx,(ny+1) % GHEIGHT) == GAREA_FOOD;
            break;
    }

    newN = to_snp(nx,ny);

    if((game_area[newN] & 0x0f) == GAREA_SNAKE) {
        game_mode = SCREEN_LOST;
        return;
    }

    if((game_area[newN] & 0x0f) == GAREA_FOOD) {
        food_active = 0;
        snake_score ++;
    } else if(game_area[newN] == GAREA_EMPTY) {
        int kk = snake_queue[sn_tpos];
        game_area[kk] = GAREA_EMPTY;
        sn_tpos = (sn_tpos + 1) % SN_QS;
    }

    /* Move snake head */
    game_area[oldN] = GAREA_SNAKE | (sn_msq_body(oldN,newN) << 4);
    game_area[newN] = GAREA_SNAKE | (sn_head_type << 4);
    sn_hpos = (sn_hpos + 1) % SN_QS;
    snake_queue[sn_hpos] = newN;

    if(!food_active) {
        put_food();
    }
}

void game_step()
{
    if(snake_ndir == snake_dir) {
        if((gKeys & KEY_L) && snake_dir != KEY_R) snake_ndir = KEY_L;
        if((gKeys & KEY_R) && snake_dir != KEY_L) snake_ndir = KEY_R;
        if((gKeys & KEY_U) && snake_dir != KEY_D) snake_ndir = KEY_U;
        if((gKeys & KEY_D) && snake_dir != KEY_U) snake_ndir = KEY_D;
    }

    if(++delay == max_delay) {
        delay = 0;
        game_update();
    }

    game_draw();
}

void lost_step()
{
    stop_event = 1; 
}

void game_menu()
{
    const char *const choices[] = {
        "New game",
        "Highscores",
        "Exit",
    };

    int choice = ui_show_menu(hwcontext, 3, choices, 0);
    switch(choice) {
        case 0:
            game_mode = SCREEN_INIT;
            break;
        case 2:
            stop_event = 1;
            break;
    }
}


void game_splash()
{
    static int ready = 0;
    if(gKeys != 0) ready = 1;
    if(gKeys == 0 && ready) game_mode = SCREEN_MENU;
    
    painter_draw_xbm(hwcontext, splash_bits, 0, 0, splash_width, splash_height);

    static const char* names[] = {
        "by Necchi",
        "by Perozzi",
        "by Sassaroli",
        "by Mascetti",
        "by Melandri",
        "by the_al",
    };

    static const int num_names = 6;
    static int cTmr = 0, cPos, st = 0, cCurName = 0;

    switch(st) {
        case 0:
            st = 1, cPos = 70;
            break;
        case 1:
            cPos -= 5;
            if(cPos == 10)
                st = 2, cTmr = 300;
            break;
        case 2:
            if(--cTmr == 0)
                st = 3;
            break;
        case 3:
            cPos -= 5;
            if(cPos <= -40)
                cCurName = (cCurName + 1) % num_names, st = 0;
            break;
    }

    //painter_draw_cropped_text(hwcontext, cPos, 5, PAINTER_SCREEN_WIDTH,
    //        5, names[cCurName], PAINTER_FONT_REGULAR,
    //        PAINTER_BLACK);
}

void game_main()
{
    switch(game_mode) {
        case SCREEN_SPLASH: game_splash(); break;
        case SCREEN_MENU:   game_menu(); break;
        case SCREEN_INIT:   game_init(); break;
        case SCREEN_GAME:   game_step(); break;
        case SCREEN_LOST:   lost_step(); break;
    }
}

//////////////// END GAME RELATED ////////////////

void snake_main(struct AppContext *appctx)
{
    hwcontext = appctx->hwcontext;

    stop_event = 0;
    while(!stop_event) {
        painter_draw_fill_rect(hwcontext, 0, 0, WIDTH-1, HEIGHT-1, 0);

        get_keys();
        game_main();

        hwcontext_update_screen(hwcontext);
        hwcontext_delay_ms(hwcontext, 30);
    }
}
