#ifndef _INPUT_H_
#define _INPUT_H_

#include <time.h>

#define EDITING_BUFFER_LEN 128
struct EditingState
{
    int last_char;
    char buffer[EDITING_BUFFER_LEN];
    int upper_case;
    char last_key;
    int last_pressed_state;
    struct timespec last_timestamp;
    int choices_length;
    int choice_idx;
    const char *char_choices;
};

void input_init_editing_state(struct EditingState *state);
void input_consume_key_event(struct EditingState *state, char key, int pressed, const struct timespec *ts);

#endif
