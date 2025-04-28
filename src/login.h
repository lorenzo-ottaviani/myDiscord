#ifndef LOGIN_H
#define LOGIN_H

#include <gtk/gtk.h>
#include "dictionaries.h"

/* All login page widget declarations */
typedef struct {
    GtkWidget *window;
    GtkWidget *main_stack;
    GtkWidget *login_title_label;
    GtkWidget *login_subtitle_label;
    GtkWidget *email_entry;
    GtkWidget *password_entry;
    GtkWidget *login_button;
    GtkWidget *create_account_msg;
    GtkWidget *register_button;
    Language current_language;
} LoginWidgets;

/* Create and return the login page widget */
GtkWidget *create_login_page(GtkWidget *stack);

#endif /* LOGIN_H */
