#ifndef _PAINTER_H_
#define _PAINTER_H_

#define PAINTER_WHITE 0
#define PAINTER_BLACK 1

void painter_clear_screen(void *hwcontext);
void painter_draw_h_line(void *hwcontext, int x, int y, int width, int color);
void painter_draw_v_line(void *hwcontext, int x, int y, int height, int color);
void painter_draw_pixel(void *hwcontext, int x, int y, int color);
void painter_draw_rect(void *hwcontext, int x, int y, int width, int height, int color);
void painter_draw_fill_rect(void *hwcontest, int x, int y, int width, int height, int color);
void painter_draw_xbm(void *hwcontext, const unsigned char *img_bits, int x, int y, int width, int height);
void painter_draw_text(void *hwcontext, int x, int row, const char *text);

#endif
