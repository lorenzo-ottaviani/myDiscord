#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "login.h"
#include "dictionaries.h"
#include "controller.h"

// check password & username function
    // collect user data & create json
    // send to db (db with compare - function in server)
    // receive response    
    
/* Callback to destroy the dialog when the user responds */
static void on_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data) {
    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    LoginWidgets *app = (LoginWidgets *)user_data;
    GtkWindow *parent_window = GTK_WINDOW(app->window);
    
    /* Retrieve text using gtk_editable_get_text in GTK4 */
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app->email_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app->password_entry));

    Translations *trans = (app->current_language == LANG_EN) ? &translations_en : &translations_fr;
    
    if (g_strcmp0(email, "user@example.com") == 0 && g_strcmp0(password, "password123") == 0) {
        g_print("Login successful!\n");
        // get user data from database
        // Transition to chat page
        gtk_stack_set_visible_child_name(GTK_STACK(app->main_stack), "chat");
    } else {
        /* Create a modal message dialog. */
        GtkWidget *dialog = gtk_message_dialog_new(parent_window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", trans->invalid_msg);
        gtk_window_set_transient_for(GTK_WINDOW(dialog), parent_window);
        g_signal_connect(dialog, "response", G_CALLBACK(on_dialog_response), NULL);
        gtk_window_present(GTK_WINDOW(dialog));
    }
}

static void on_register_button_clicked(GtkButton *button, gpointer user_data) {
    LoginWidgets *app = (LoginWidgets *)user_data;
    g_print("Register button clicked!\n");
    // Transistion to registration page
    gtk_stack_set_visible_child_name(GTK_STACK(app->main_stack), "signup");
}

/* Create the login page widget.
* The login page UI is built inside a vertical box.
* The "stack" parameter is a pointer to the GtkStack that holds
* all pages stored in the LoginWidgets struct for callbacks.
*/

GtkWidget* create_login_page(GtkWidget *stack, GtkWidget *window) {
    LoginWidgets *app = malloc(sizeof(LoginWidgets));
    if (!app) {
        g_error("Failed to allocate LoginWidgets");
        }

    // assign the main window pointer
    app->window = window;
    /* Default language set to English */
    app->current_language = LANG_EN;
    /* Save the pointer to the main stack */
    app->main_stack = stack;

    /* Create a top-level vertical box (vbox) to hold sections */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 40);
    /* Set appropriate margins... */
    gtk_widget_set_margin_top(vbox, 20);
    gtk_widget_set_margin_bottom(vbox, 20);
    gtk_widget_set_margin_start(vbox, 20);
    gtk_widget_set_margin_end(vbox, 20);
   
    /* --- Language Toggle Section --- */
    GtkWidget *lang_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(lang_box, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(vbox), lang_box);

    GtkWidget *lang_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "EN");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "FR");
    gtk_combo_box_set_active(GTK_COMBO_BOX(lang_combo), 0);
    g_signal_connect(lang_combo, "changed", G_CALLBACK(on_login_language_changed), app);
    gtk_box_append(GTK_BOX(lang_box), lang_combo);

    /* --- Section 1: Title --- */
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), title_box);
    
    app->login_title_label = gtk_label_new(NULL);
    // Set the initial title markup: bold and large.
    char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>", translations_en.login_title);
    gtk_label_set_markup(GTK_LABEL(app->login_title_label), markup_title);
    g_free(markup_title);
    gtk_widget_set_halign(app->login_title_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), app->login_title_label);
    
    app->login_subtitle_label = gtk_label_new(translations_en.login_subtitle);
    gtk_widget_set_halign(app->login_subtitle_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), app->login_subtitle_label);

    /* --- Section 2: Login --- */
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), login_box);
    
    app->email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->email_entry), translations_en.email_placeholder);
    gtk_widget_set_size_request(app->email_entry, 350, -1);
    gtk_widget_set_hexpand(app->email_entry, FALSE);
    gtk_widget_set_halign(app->email_entry, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(login_box), app->email_entry);
    
    app->password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(app->password_entry), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->password_entry), translations_en.password_placeholder);
    gtk_widget_set_size_request(app->password_entry, 350, -1);
    gtk_widget_set_hexpand(app->password_entry, FALSE);
    gtk_widget_set_halign(app->password_entry, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(login_box), app->password_entry);

    app->login_button = gtk_button_new_with_label(translations_en.login_button);
    gtk_widget_set_size_request(app->login_button, 150, -1);
    gtk_widget_set_halign(app->login_button, GTK_ALIGN_CENTER);
    g_signal_connect(app->login_button, "clicked", G_CALLBACK(on_login_button_clicked), app);
    gtk_box_append(GTK_BOX(login_box), app->login_button);

    /* --- Section 3: Registration --- */
    GtkWidget *reg_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), reg_box);
    
    app->create_account_msg = gtk_label_new(translations_en.create_account_msg);
    gtk_widget_set_halign(app->create_account_msg, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(reg_box), app->create_account_msg);
    
    app->register_button = gtk_button_new_with_label(translations_en.register_button);
    gtk_widget_set_size_request(app->register_button, 150, -1);
    gtk_widget_set_halign(app->register_button, GTK_ALIGN_CENTER);
    g_signal_connect(app->register_button, "clicked", G_CALLBACK(on_register_button_clicked), app);
    gtk_box_append(GTK_BOX(reg_box), app->register_button);

    gtk_window_present(GTK_WINDOW(app->window));

    return vbox;
}
