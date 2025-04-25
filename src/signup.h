#ifndef SIGNUP_H
#define SIGNUP_H

#include <gtk/gtk.h>
#include "dictionaries.h"

/* All signup page widget declarations */
typedef struct {
    GtkWidget *window;
    GtkWidget *main_stack;
    GtkWidget *register_title_label;
    GtkWidget *register_subtitle_label;
    
    GtkWidget *name_entry;
    GtkWidget *surname_entry;
    GtkWidget *username_entry;
    GtkWidget *email_entry;
    GtkWidget *password_entry;
    
    GtkWidget *invalid_msg;
    GtkWidget *return_msg;
    
    GtkWidget *register_button;
    GtkWidget *return_button;
    Language current_language;
} SignupWidgets;

/* Create and return the signup page widget */
GtkWidget *create_signup_page(void);

#endif /* SIGNUP_H */
