#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "signup.h"
#include "dictionaries.h"
#include "controller.h"

/* Callback to destroy the dialog when the user responds */
static void on_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data) {
    gtk_window_destroy(GTK_WINDOW(dialog));
}

/* Callback for when the register button is clicked */
static void on_register_button_clicked(GtkButton *button, gpointer user_data) {
    SignupWidgets *widget = (SignupWidgets *)user_data;
    GtkWindow *parent_window = GTK_WINDOW(widget->window);
    
    /* Retrieve text using gtk_editable_get_text (works also for GTK4) */
    const char *name = gtk_editable_get_text(GTK_EDITABLE(widget->name_entry));
    const char *surname = gtk_editable_get_text(GTK_EDITABLE(widget->surname_entry));
    const char *username = gtk_editable_get_text(GTK_EDITABLE(widget->username_entry));
    const char *email = gtk_editable_get_text(GTK_EDITABLE(widget->email_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(widget->password_entry));

    Translations *trans = (widget->current_language == LANG_EN) ? &translations_en : &translations_fr;
    
    /* Sample validations (replace with actual validations):
     * Here you would check whether each field is not empty, whether the username or email already exists,
     * or whether the password meets length requirements.
     */
    if (strlen(name) > 0 && strlen(surname) > 0 && strlen(username) > 0 &&
        strlen(email) > 0 && strlen(password) > 0 /* && additional database checks here */) {
        g_print("Registration successful!\n");
        // Go back to login screen or continue with saving to server.
    } else {
        /* Create a modal message dialog with the invalid_message string */
        GtkWidget *dialog = gtk_message_dialog_new(parent_window,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_CLOSE,
                                                   "%s",
                                                   trans->invalid_msg);
        gtk_window_set_transient_for(GTK_WINDOW(dialog), parent_window);
        g_signal_connect(dialog, "response", G_CALLBACK(on_dialog_response), NULL);
        gtk_window_present(GTK_WINDOW(dialog));
    }
}

/* Callback for when the return button (cancel) is clicked */
static void on_return_button_clicked(GtkButton *button, gpointer user_data) {
    /* Implement the action to return or cancel registration.
       For example, you might destroy the registration window and open the login window again. */
    g_print("Return button clicked, going back to the login screen...\n");
}

// /* Callback for language combo box changed */
// static void on_language_changed(GtkComboBoxText *combo, gpointer user_data) {
//     SignupWidgets *app = (SignupWidgets *)user_data;
    
//     /* gtk_combo_box_text_get_active_text() returns a newly allocated string */
//     gchar *lang = gtk_combo_box_text_get_active_text(combo);
//     if (lang) {
//         if (strcmp(lang, "EN") == 0)
//             app->current_language = LANG_EN;
//         else
//             app->current_language = LANG_FR;
//         g_free(lang);
//     }
    
//     update_signup_ui_texts(app);
// }

/* Build the UI */
static void activate(GtkApplication *app_inst, gpointer user_data) {

    SignupWidgets *widgets = malloc(sizeof(SignupWidgets));

    /* Default language set to English */
    widgets->current_language = LANG_EN;

    apply_css();

    widgets->window = gtk_application_window_new(app_inst);
    gtk_window_set_title(GTK_WINDOW(widgets->window), "MyDiscord");
    gtk_window_set_default_size(GTK_WINDOW(widgets->window), 400, 500);

    /* Create a top-level vertical box (vbox) to hold sections */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 40);
    gtk_widget_set_margin_top(vbox, 20);
    gtk_widget_set_margin_bottom(vbox, 20);
    gtk_widget_set_margin_start(vbox, 20);
    gtk_widget_set_margin_end(vbox, 20);
    gtk_window_set_child(GTK_WINDOW(widgets->window), vbox);

    /* --- Language Toggle Section --- */
    GtkWidget *lang_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(lang_box, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(vbox), lang_box);

    GtkWidget *lang_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "EN");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "FR");
    gtk_combo_box_set_active(GTK_COMBO_BOX(lang_combo), 0);
    g_signal_connect(lang_combo, "changed", G_CALLBACK(on_language_changed), widgets);
    gtk_box_append(GTK_BOX(lang_box), lang_combo);

    /* --- Section 1: Title --- */
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), title_box);
    
    widgets->register_title_label = gtk_label_new(NULL);
    // Set the initial title markup: bold and large.
    char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>",
                                                   translations_en.register_title);
    gtk_label_set_markup(GTK_LABEL(widgets->register_title_label), markup_title);
    g_free(markup_title);
    gtk_widget_set_halign(widgets->register_title_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), widgets->register_title_label);
    
    widgets->register_subtitle_label = gtk_label_new(translations_en.register_subtitle);
    gtk_widget_set_halign(widgets->register_subtitle_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), widgets->register_subtitle_label);

    /* --- Section 2: Registration --- */
    GtkWidget *reg_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), reg_box);

    widgets->name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->name_entry), translations_en.name_placeholder);
    gtk_widget_set_size_request(widgets->name_entry, 300, -1);
    gtk_box_append(GTK_BOX(reg_box), widgets->name_entry);

    widgets->surname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->surname_entry), translations_en.surname_placeholder);
    gtk_widget_set_size_request(widgets->surname_entry, 300, -1);
    gtk_box_append(GTK_BOX(reg_box), widgets->surname_entry);

    widgets->username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->username_entry), translations_en.username_placeholder);
    gtk_widget_set_size_request(widgets->username_entry, 300, -1);
    gtk_box_append(GTK_BOX(reg_box), widgets->username_entry);
    
    widgets->email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->email_entry), translations_en.email_placeholder);
    gtk_widget_set_size_request(widgets->email_entry, 300, -1);
    gtk_box_append(GTK_BOX(reg_box), widgets->email_entry);
    
    widgets->password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(widgets->password_entry), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->password_entry), translations_en.password_placeholder);
    gtk_widget_set_size_request(widgets->password_entry, 300, -1);
    gtk_box_append(GTK_BOX(reg_box), widgets->password_entry);

    widgets->register_button = gtk_button_new_with_label(translations_en.register_button);
    gtk_widget_set_size_request(widgets->register_button, 150, -1);
    gtk_widget_set_halign(widgets->register_button, GTK_ALIGN_CENTER);
    g_signal_connect(widgets->register_button, "clicked", G_CALLBACK(on_register_button_clicked), widgets);
    gtk_box_append(GTK_BOX(reg_box), widgets->register_button);

    /* --- Section 3: Cancel --- */
    GtkWidget *cancel_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), cancel_box);
    
    widgets->return_msg = gtk_label_new(translations_en.return_msg);
    gtk_widget_set_halign(widgets->return_msg, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(cancel_box), widgets->return_msg);
    
    widgets->return_button = gtk_button_new_with_label(translations_en.return_button);
    gtk_widget_set_size_request(widgets->return_button, 150, -1);
    gtk_widget_set_halign(widgets->return_button, GTK_ALIGN_CENTER);
    g_signal_connect(widgets->return_button, "clicked", G_CALLBACK(on_return_button_clicked), widgets);
    gtk_box_append(GTK_BOX(cancel_box), widgets->return_button);

    gtk_window_present(GTK_WINDOW(widgets->window));
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
