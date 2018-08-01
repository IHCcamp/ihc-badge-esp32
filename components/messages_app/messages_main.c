#include "appcontext.h"
#include "message.h"
#include "hwcontext.h"
#include "painter.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int count_messages(const struct AppContext *appctx);
static void populate_with_subjects(const struct AppContext *appctx, char **messages, int len);
static char *get_message(const struct AppContext *appctx, int message_index);

void messages_main(struct AppContext *appctx)
{
    void *hwcontext = appctx->hwcontext;

    int messages_count = count_messages(appctx);
    char **messages = NULL;

    if (messages_count > 0) {
        messages = calloc(messages_count, sizeof(char *));
        populate_with_subjects(appctx, messages, messages_count);

        int choice = 0;
        do {
            choice = ui_show_menu(hwcontext, messages_count, (const char * const*) messages, choice);
            if (choice >= 0) {
                char *message_text = get_message(appctx, choice);
                ui_show_message(hwcontext, message_text);
                free(message_text);
            }
        } while (choice >= 0);
    } else {
        ui_show_message(hwcontext, "No messages");
    }

    for (int i = 0; i < messages_count; i++) {
        free(messages[i]);
    }
    free(messages);
}

static int count_messages(const struct AppContext *appctx)
{
    if (!appctx->msgs) {
        return 0;
    }

    int messages_count = 0;

    struct Message *message = GET_LIST_ENTRY(appctx->msgs, struct Message, message_list_head);
    struct Message *first_message = message;
    do {
        messages_count++;
        message = GET_LIST_ENTRY(message->message_list_head.next, struct Message, message_list_head);
    } while (message != first_message);

    return messages_count;
}

static void populate_with_subjects(const struct AppContext *appctx, char **messages, int len)
{
    struct Message *message = GET_LIST_ENTRY(appctx->msgs, struct Message, message_list_head);
    for (int i = 0; i < len; i++) {
        messages[i] = strdup(message->topic);
        message = GET_LIST_ENTRY(message->message_list_head.next, struct Message, message_list_head);
    }
}

static char *get_message(const struct AppContext *appctx, int message_index)
{
    struct Message *message = GET_LIST_ENTRY(appctx->msgs, struct Message, message_list_head);
    while (message_index) {
        message = GET_LIST_ENTRY(message->message_list_head.next, struct Message, message_list_head);
        message_index--;
    }
    return strndup(message->data, message->data_len);
}
