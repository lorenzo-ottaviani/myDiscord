#ifndef CHAT_H
#define CHAT_H

#include <gtk/gtk.h>
#include "dictionaries.h"

/* All chat page widget declarations */
typedef struct {
    GtkWidget *window;
    GtkWidget *main_stack;
    GtkWidget *main_channel_title_label;

    GtkWidget *header_box;      // Contains the large channel name/user info/toggle and logout on main page
    GtkWidget *channel_list;    // Container for channel items (left column) of main page
    GtkWidget *message_list;    // Container for messages inside the scrollable chat area on main page
    GtkWidget *user_list;       // Container for user info

    GtkWidget *user_info_label; // Label for showing username with status

    GtkWidget *online_status;
    GtkWidget *offline_status;
    GtkWidget *away_status;

    GtkWidget *message_entry;

    GtkWidget *logout_button;
    GtkWidget *send_button;
    Language current_language;
} ChatWidgets;

/* Create and return the chat page widget */
GtkWidget *create_chat_page(void);

#endif /* CHAT_H */
