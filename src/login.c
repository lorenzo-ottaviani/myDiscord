#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "dictionaries.h"  // our own translation dictionary

/* Structure to hold our UI widgets that need updating */
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
} AppWidgets;

/* Function to update all UI texts based on the selected language */
static void update_ui_texts(AppWidgets *app) {
    Translations *trans;
    
    if (app->current_language == LANG_EN) {
        trans = &translations_en;
    } else {
        trans = &translations_fr;
    }
    
    // Create markup for the title (bigger and bolder)
    char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>",
    trans->login_title);
    gtk_label_set_markup(GTK_LABEL(app->login_title_label), markup_title);
    g_free(markup_title);

    gtk_label_set_text(GTK_LABEL(app->login_subtitle_label), trans->login_subtitle);
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->email_entry), trans->email_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->password_entry), trans->password_placeholder);
    gtk_button_set_label(GTK_BUTTON(app->login_button), trans->login_button);
    gtk_label_set_text(GTK_LABEL(app->create_account_msg), trans->create_account_msg);
    gtk_button_set_label(GTK_BUTTON(app->register_button), trans->register_button);
}

/* Callback to destroy the dialog when the user responds */
static void on_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data){
    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    AppWidgets *app = (AppWidgets *)user_data;
    GtkWindow *parent_window = GTK_WINDOW(app->window);
    
    /* Retrieve text using gtk_editable_get_text in GTK4 */
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app->email_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app->password_entry));

    Translations *trans = (app->current_language == LANG_EN) ? &translations_en : &translations_fr;
    
    if (g_strcmp0(email, "user@example.com") == 0 &&
        g_strcmp0(password, "password123") == 0) {
        g_print("Login successful!\n");
        // Proceed to chat application main page
    } else {
        /* Create a modal message dialog.
         * Note: We use "%s" as format string to avoid the format-security warning.
         */
        GtkWidget *dialog = gtk_message_dialog_new(parent_window,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_CLOSE,
                                                   "%s",
                                                   trans->invalid_message);
        gtk_window_set_transient_for(GTK_WINDOW(dialog), parent_window);
        g_signal_connect(dialog, "response", G_CALLBACK(on_dialog_response), NULL);
        gtk_window_present(GTK_WINDOW(dialog));
    }
}

static void on_register_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Register button clicked!\n");
    // Open registration window; implementation as needed.
}

/* Callback for language combo box changed */
static void on_language_changed(GtkComboBoxText *combo, gpointer user_data) {
    AppWidgets *app = (AppWidgets *)user_data;
    const char *lang = gtk_combo_box_text_get_active_text(combo);
    
    if (strcmp(lang, "EN") == 0)
        app->current_language = LANG_EN;
    else
        app->current_language = LANG_FR;
    
    update_ui_texts(app);
}

/* Build the UI */
static void activate(GtkApplication *app_inst, gpointer user_data) {
    AppWidgets *app_widgets = malloc(sizeof(AppWidgets));
    /* Default language set to English */
    app_widgets->current_language = LANG_EN;

    app_widgets->window = gtk_application_window_new(app_inst);
    gtk_window_set_title(GTK_WINDOW(app_widgets->window), "MyDiscord");
    gtk_window_set_default_size(GTK_WINDOW(app_widgets->window), 400, 500);

    /* Create a top-level vertical box (vbox) to hold sections */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 40);
    gtk_widget_set_margin_top(vbox, 20);
    gtk_widget_set_margin_bottom(vbox, 20);
    gtk_widget_set_margin_start(vbox, 20);
    gtk_widget_set_margin_end(vbox, 20);
    gtk_window_set_child(GTK_WINDOW(app_widgets->window), vbox);

    /* --- Language Toggle Section --- */
    GtkWidget *lang_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(lang_box, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(vbox), lang_box);

    GtkWidget *lang_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "EN");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "FR");
    gtk_combo_box_set_active(GTK_COMBO_BOX(lang_combo), 0);
    g_signal_connect(lang_combo, "changed", G_CALLBACK(on_language_changed), app_widgets);
    gtk_box_append(GTK_BOX(lang_box), lang_combo);

    /* --- Section 1: Title --- */
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), title_box);
    
    app_widgets->login_title_label = gtk_label_new(NULL);
    // Set the initial title markup: bold and large.
    char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>", translations_en.login_title);
    gtk_label_set_markup(GTK_LABEL(app_widgets->login_title_label), markup_title);
    g_free(markup_title);
    gtk_widget_set_halign(app_widgets->login_title_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), app_widgets->login_title_label);
    
    app_widgets->login_subtitle_label = gtk_label_new(translations_en.login_subtitle);
    gtk_widget_set_halign(app_widgets->login_subtitle_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), app_widgets->login_subtitle_label);

    /* --- Section 2: Login --- */
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), login_box);
    
    app_widgets->email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_widgets->email_entry), translations_en.email_placeholder);
    gtk_widget_set_size_request(app_widgets->email_entry, 300, -1);
    gtk_box_append(GTK_BOX(login_box), app_widgets->email_entry);
    
    app_widgets->password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(app_widgets->password_entry), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_widgets->password_entry), translations_en.password_placeholder);
    gtk_widget_set_size_request(app_widgets->password_entry, 300, -1);
    gtk_box_append(GTK_BOX(login_box), app_widgets->password_entry);

    app_widgets->login_button = gtk_button_new_with_label(translations_en.login_button);
    gtk_widget_set_size_request(app_widgets->login_button, 150, -1);
    gtk_widget_set_halign(app_widgets->login_button, GTK_ALIGN_CENTER);
    g_signal_connect(app_widgets->login_button, "clicked", G_CALLBACK(on_login_button_clicked), app_widgets);
    gtk_box_append(GTK_BOX(login_box), app_widgets->login_button);

    /* --- Section 3: Registration --- */
    GtkWidget *reg_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), reg_box);
    
    app_widgets->create_account_msg = gtk_label_new(translations_en.create_account_msg);
    gtk_widget_set_halign(app_widgets->create_account_msg, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(reg_box), app_widgets->create_account_msg);
    
    app_widgets->register_button = gtk_button_new_with_label(translations_en.register_button);
    gtk_widget_set_size_request(app_widgets->register_button, 150, -1);
    gtk_widget_set_halign(app_widgets->register_button, GTK_ALIGN_CENTER);
    g_signal_connect(app_widgets->register_button, "clicked", G_CALLBACK(on_register_button_clicked), NULL);
    gtk_box_append(GTK_BOX(reg_box), app_widgets->register_button);

    gtk_window_present(GTK_WINDOW(app_widgets->window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.github.leila-wilde.MyDiscord", 0);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

