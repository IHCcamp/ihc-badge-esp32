#include "input.h"
#include <ctype.h>
#include <string.h>

#define RELEASED_STATE 0
#define PRESSED_STATE 1

#define KEY_TIMEOUT_MS 800

static const char key_1_choices[] = {'.', ',', ':', '!', '?', '"', ';'};
static const char key_2_choices[] = {'a', 'b', 'c'};
static const char key_3_choices[] = {'d', 'e', 'f'};
static const char key_4_choices[]= {'g', 'h', 'i'};
static const char key_5_choices[]= {'j', 'k', 'l'};
static const char key_6_choices[]= {'m', 'n', 'o'};
static const char key_7_choices[]= {'p', 'q', 'r', 's'};
static const char key_8_choices[]= {'t', 'u', 'v'};
static const char key_9_choices[]= {'w', 'x', 'y', 'z'};
static const char key_0_choices[]= {' '};
static const char key_star_choices[]= {'+'};

static int is_input_key(char key) {
    switch(key) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
        case '*':
            return 1;
    }
    return 0;
}

static void set_key_choices(struct EditingState *state, char key)
{
    if (!is_input_key(key)) {
        return;
    }

    // Reset choice idx
    state->choice_idx = 0;

    switch (key) {
        case '1':
            state->char_choices = key_1_choices;
            state->choices_length = sizeof(key_1_choices);
            break;

        case '2':
            state->char_choices = key_2_choices;
            state->choices_length = sizeof(key_2_choices);
            break;

        case '3':
            state->char_choices = key_3_choices;
            state->choices_length = sizeof(key_3_choices);
            break;

        case '4':
            state->char_choices = key_4_choices;
            state->choices_length = sizeof(key_4_choices);
            break;

        case '5':
            state->char_choices = key_5_choices;
            state->choices_length = sizeof(key_5_choices);
            break;

        case '6':
            state->char_choices = key_6_choices;
            state->choices_length = sizeof(key_6_choices);
            break;

        case '7':
            state->char_choices = key_7_choices;
            state->choices_length = sizeof(key_7_choices);
            break;

        case '8':
            state->char_choices = key_8_choices;
            state->choices_length = sizeof(key_8_choices);
            break;

        case '9':
            state->char_choices = key_9_choices;
            state->choices_length = sizeof(key_9_choices);
            break;

        case '0':
            state->char_choices = key_0_choices;
            state->choices_length = sizeof(key_0_choices);
            break;

        case '*':
            state->char_choices = key_star_choices;
            state->choices_length = sizeof(key_star_choices);
            break;
    }
}

static int timespec_ms_diff(const struct timespec *ts1, const struct timespec *ts2)
{
    return (ts1->tv_sec - ts2->tv_sec) * 1000 + (ts1->tv_nsec - ts2->tv_nsec) / 1000000;
}

static void increase_last_char(struct EditingState *state)
{
    state->last_char++;
    if (state->last_char >= EDITING_BUFFER_LEN) {
        state->last_char = EDITING_BUFFER_LEN - 1;
    }
}

static void decrease_last_char(struct EditingState *state)
{
    state->last_char--;
    if (state->last_char <= -1) {
        state->last_char = -1;
    }
}

static void set_current_char(struct EditingState *state, char c)
{
    if (state->last_char < 0 || state->last_char >= EDITING_BUFFER_LEN) {
        return;
    }

    state->buffer[state->last_char] = c;
}

void input_init_editing_state(struct EditingState *state)
{
    struct timespec ts = {
        .tv_sec = 0,
        .tv_nsec = 0
    };
    state->last_char = -1;
    state->upper_case = 0;
    state->last_key = '\x00';
    state->last_pressed_state = -1;
    state->last_timestamp = ts;
    memset(state->buffer, 0, EDITING_BUFFER_LEN);
}

void input_consume_key_event(struct EditingState *state, char key, int pressed, const struct timespec *ts)
{
    // Input on release
    if (pressed != RELEASED_STATE) {
        return;
    }

    // Special keys
    if (key == '#') {
        state->upper_case = !state->upper_case;
    } else if (key == 'C') {
        set_current_char(state, '\x00');
        decrease_last_char(state);
    }

    // Handle input keys
    if (is_input_key(key)) {
        if (key != state->last_key
                || state->choices_length == 1 // Single choice key always commits
                || timespec_ms_diff(ts, &state->last_timestamp) > KEY_TIMEOUT_MS) {
            increase_last_char(state);
            set_key_choices(state, key);
        } else {
            state->choice_idx++;
            if (state->choice_idx >= state->choices_length) {
                state->choice_idx = 0;
            }
        }

        // Has to be unsigned otherwise toupper fails
        unsigned char input = state->char_choices[state->choice_idx];
        if (state->upper_case) {
            input = toupper(input);
        }
        set_current_char(state, input);
    }

    state->last_key = key;
    state->last_pressed_state = pressed;
    state->last_timestamp = *ts;
}
