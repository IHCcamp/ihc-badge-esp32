#ifndef _UI_H_
#define _UI_H_

void ui_show_message(void *hwcontext, const char *message);
int ui_show_menu(void *hwcontext, int num_entries, const char * const *entries, int current_entry);
void ui_print_menu_button_label(void *hwcontext, const char *label);
char *ui_ask_user_input(void *hwcontext, const char *message);
void ui_draw_animation(void *hwcontext, int framescount, int fps, const unsigned char *img_bits, int x, int y, int width, int height);

#endif
