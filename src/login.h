#ifndef LOGIN_H
#define LOGIN_H

#include <gtk/gtk.h>
#include "controller.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *login_title_label;
    GtkWidget *login_subtitle_label;
    GtkWidget *email_entry;
    GtkWidget *password_entry;
    GtkWidget *login_button;
    GtkWidget *create_account_msg;
    GtkWidget *register_button;
    Language current_language;
} LoginWidgets;

void update_login_ui_texts(LoginWidgets *loginWidget) {
    Translations *trans = (loginWidget->current_language == LANG_EN)
                            ? &translations_en : &translations_fr;

    if (loginWidget->login_title_label) {
        /* Create markup for the title (bigger and bolder) */
        char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>", 
                                                       trans->login_title);
        gtk_label_set_markup(GTK_LABEL(loginWidget->login_title_label), markup_title);
        g_free(markup_title);
    }

    gtk_label_set_text(GTK_LABEL(loginWidget->login_subtitle_label), trans->login_subtitle);
    gtk_label_set_text(GTK_LABEL(loginWidget->create_account_msg), trans->create_account_msg);

    gtk_entry_set_placeholder_text(GTK_ENTRY(loginWidget->email_entry), trans->email_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(loginWidget->password_entry), trans->password_placeholder);
 
    gtk_button_set_label(GTK_BUTTON(loginWidget->register_button), trans->register_button);
    gtk_button_set_label(GTK_BUTTON(loginWidget->login_button), trans->login_button);
}

/* Callback for language combo box changed */
static void on_language_changed(GtkComboBoxText *combo, gpointer user_data) {
    LoginWidgets *app = (LoginWidgets *)user_data;
    const char *lang = gtk_combo_box_text_get_active_text(combo);
    
    if (strcmp(lang, "EN") == 0)
        app->current_language = LANG_EN;
    else
        app->current_language = LANG_FR;
    
    update_login_ui_texts(app);
}

/* Declaration for a function to create the login page if needed */
GtkWidget *create_login_page(void);

#endif
