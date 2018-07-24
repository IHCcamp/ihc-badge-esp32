#ifndef _PAINTER_H_
#define _PAINTER_H_

#define PAINTER_WHITE 0
#define PAINTER_BLACK 1

#define PAINTER_FONT_REGULAR 0
#define PAINTER_FONT_BOLD 1

#define PAINTER_SCREEN_WIDTH 84
#define PAINTER_SCREEN_HEIGHT 48

void painter_clear_screen(void *hwcontext);
void painter_draw_h_line(void *hwcontext, int x, int y, int width, int color);
void painter_draw_v_line(void *hwcontext, int x, int y, int height, int color);
void painter_draw_pixel(void *hwcontext, int x, int y, int color);
void painter_draw_rect(void *hwcontext, int x, int y, int width, int height, int color);
void painter_draw_fill_rect(void *hwcontest, int x, int y, int width, int height, int color);
void painter_draw_xbm(void *hwcontext, const unsigned char *img_bits, int x, int y, int width, int height);
void painter_draw_bounded_text(void *hwcontext, int x, int row, int x_bound, int y_row_bound, const char *text, int style, int color);
void painter_draw_text(void *hwcontext, int x, int row, const char *text, int style, int color);
int painter_painted_text_width(const char *text, int style);

#endif
