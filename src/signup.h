#ifndef SIGNUP_H
#define SIGNUP_H

#include <gtk/gtk.h>
#include <gtk/gtk.h>
#include "controller.h"

typedef struct {
    GtkWidget *window;
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

void update_signup_ui_texts(SignupWidgets *widget) {
    Translations *trans = (widget->current_language == LANG_EN)
                            ? &translations_en : &translations_fr;

    if (widget->register_title_label) {
        /* Create markup for the title (bigger and bolder) */
        char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>", 
                                                       trans->register_title);
        gtk_label_set_markup(GTK_LABEL(widget->register_title_label), markup_title);
        g_free(markup_title);
    }
    
    gtk_label_set_text(GTK_LABEL(widget->register_title_label), trans->register_title);
    gtk_label_set_text(GTK_LABEL(widget->register_subtitle_label), trans->register_subtitle);
    
    gtk_label_set_text(GTK_LABEL(widget->invalid_msg), trans->invalid_msg);
    gtk_label_set_text(GTK_LABEL(widget->return_msg), trans->return_msg);

    gtk_entry_set_placeholder_text(GTK_ENTRY(widget->name_entry), trans->name_placeholder);     
    gtk_entry_set_placeholder_text(GTK_ENTRY(widget->surname_entry), trans->surname_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(widget->username_entry), trans->username_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(widget->email_entry), trans->email_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(widget->password_entry), trans->password_placeholder);
 
    gtk_button_set_label(GTK_BUTTON(widget->register_button), trans->register_button);
    gtk_button_set_label(GTK_BUTTON(widget->return_button), trans->return_button);
}

/* Callback for language combo box changed */
static void on_language_changed(GtkComboBoxText *combo, gpointer user_data) {
    SignupWidgets *app = (SignupWidgets *)user_data;
    const char *lang = gtk_combo_box_text_get_active_text(combo);
    
    if (strcmp(lang, "EN") == 0)
        app->current_language = LANG_EN;
    else
        app->current_language = LANG_FR;
    
    update_signup_ui_texts(app);
}

// static void on_register_button_clicked(GtkButton *button, gpointer user_data) {
//     SignupWidgets *app_ctr = (SignupWidgets *)user_data;
//     g_print("Registration successful!\n");

//     GtkWidget *create_signup_page(void);
// }

#endif