#ifndef CHAT_H
#define CHAT_H

#include <gtk/gtk.h>
#include "dictionaries.h"

/* All chat page widget declarations */
typedef struct {
    GtkWidget *window;
    GtkWidget *main_stack;
    GtkWidget *main_channel_title_label;

    GtkWidget *header_box;
    GtkWidget *channel_list;
    GtkWidget *message_list;
    GtkWidget *user_list;
    GtkWidget *user_info_label;

    GtkWidget *online_status;
    GtkWidget *offline_status;
    GtkWidget *away_status;

    GtkWidget *message_entry;

    GtkWidget *logout_button;
    GtkWidget *send_button;
    Language current_language;
} ChatWidgets;

/* Create and return the chat page widget */
GtkWidget *create_chat_page(GtkWidget *stack, GtkWidget *window);

#endif /* CHAT_H */
