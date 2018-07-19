#ifndef _PAINTER_H_
#define _PAINTER_H_

#define PAINTER_WHITE 0
#define PAINTER_BLACK 1

void painter_draw_h_line(void *hwcontext, int x, int y, int width, int color);
void painter_draw_v_line(void *hwcontext, int x, int y, int height, int color);
void painter_draw_pixel(void *hwcontext, int x, int y, int color);
void painter_draw_rect(void *hwcontext, int x, int y, int width, int height, int color);

#endif
