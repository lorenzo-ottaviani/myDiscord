/* 
"controller" API / context structure (e.g., AppWidgets) 
contains pointers to the stack and other shared data
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gtk/gtk.h>
#include "dictionaries.h"

/* Structure to hold our UI widgets that need updating */
typedef struct {
    GtkWidget *window;
    GtkWidget *main_stack;    // pointer to the GtkStack widget
    GtkWidget *login_title_label;
    GtkWidget *login_subtitle_label;
    GtkWidget *register_title_label;
    GtkWidget *register_subtitle_label;
    GtkWidget *main_channel_title_label;

    GtkWidget *name_entry;
    GtkWidget *surname_entry;
    GtkWidget *username_entry;
    GtkWidget *email_entry;
    GtkWidget *password_entry;
    GtkWidget *message_entry;

    GtkWidget *header_box;      // Contains the large channel name/user info/toggle and logout on main page
    GtkWidget *channel_list;    // Container for channel items (left column) of main page
    GtkWidget *message_list;    // Container for messages inside the scrollable chat area on main page
    GtkWidget *user_list;       // Container for user info

    GtkWidget *user_info_label; // Label for showing username with status

    GtkWidget *online_status;
    GtkWidget *offline_status;
    GtkWidget *away_status;

    GtkWidget *register_button;
    GtkWidget *return_button;
    GtkWidget *login_button;
    GtkWidget *logout_button;
    GtkWidget *send_button;

    GtkWidget *create_account_msg;
    GtkWidget *return_msg;

    Language current_language;

} AppWidgets;

/* CSS styling function */
static void apply_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css_file = "myDiscord.css";

    gtk_css_provider_load_from_path(provider, css_file);
    
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

// /* Function to update all UI texts based on the selected language */
// static void update_ui_texts(AppWidgets *app) {
//     Translations *trans;

//     if (app->current_language == LANG_EN) {
//         trans = &translations_en;
//     } else {
//         trans = &translations_fr;
//     }

//     // Create markup for the title (bigger and bolder)
//     char *markup_title2 = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>", trans->register_title);
//     gtk_label_set_markup(GTK_LABEL(app->register_title_label), markup_title2);
//     g_free(markup_title2);


//     // Create markup for the title (bigger and bolder)
//     char *markup_title3 = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>", trans->main_channel_title);
//     gtk_label_set_markup(GTK_LABEL(app->main_channel_title_label), markup_title3);
//     g_free(markup_title3);

//     // Update subtitle and other placeholders accordingly.
//     gtk_label_set_text(GTK_LABEL(app->register_subtitle_label), trans->register_subtitle);
//     gtk_label_set_text(GTK_LABEL(app->return_msg), trans->return_msg);

//     gtk_entry_set_placeholder_text(GTK_ENTRY(app->name_entry), trans->name_placeholder);
//     gtk_entry_set_placeholder_text(GTK_ENTRY(app->surname_entry), trans->surname_placeholder);
//     gtk_entry_set_placeholder_text(GTK_ENTRY(app->username_entry), trans->username_placeholder);
//     gtk_entry_set_placeholder_text(GTK_ENTRY(app->email_entry), trans->email_placeholder);
//     gtk_entry_set_placeholder_text(GTK_ENTRY(app->password_entry), trans->password_placeholder);
//     gtk_entry_set_placeholder_text(GTK_ENTRY(app->message_entry), trans->message_placeholder);

//     gtk_button_set_label(GTK_BUTTON(app->return_button), trans->return_button);
//     gtk_button_set_label(GTK_BUTTON(app->login_button), trans->login_button);
//     gtk_button_set_label(GTK_BUTTON(app->logout_button), trans->logout_button);
//     gtk_button_set_label(GTK_BUTTON(app->send_button), trans->send_button);
    
// }

// /* Callback for language combo box changed */
// static void on_language_changed(GtkComboBoxText *combo, gpointer user_data) {
//     AppWidgets *app = (AppWidgets *)user_data;
//     const char *lang = gtk_combo_box_text_get_active_text(combo);
    
//     if (strcmp(lang, "EN") == 0)
//         app->current_language = LANG_EN;
//     else
//         app->current_language = LANG_FR;
    
//     update_ui_texts(app);
// }


#endif