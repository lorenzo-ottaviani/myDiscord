#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gtk/gtk.h>



void apply_css(void);

void update_login_ui_texts(void *widget_struct);
void update_signup_ui_texts(void *widget_struct);
void update_chat_ui_texts(void *widget_struct);

void on_login_language_changed(GtkComboBoxText *combo, gpointer user_data);
void on_signup_language_changed(GtkComboBoxText *combo, gpointer user_data);
void on_chat_language_changed(GtkComboBoxText *combo, gpointer user_data);

#endif /* CONTROLLER_H */
