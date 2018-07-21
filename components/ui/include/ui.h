#ifndef _UI_H_
#define _UI_H_

void ui_show_message(void *hwcontext, const char *message);
int ui_show_menu(void *hwcontext, int num_entries, const char * const *entries, int current_entry);

#endif
