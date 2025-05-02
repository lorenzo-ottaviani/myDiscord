#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // for sleep/close()
#include "signup.h"
#include "dictionaries.h"
#include "controller.h"

// Global variable for the socket file descriptor
extern int sockfd;

/* Callback for when the register button is clicked */
static void on_register_button_clicked(GtkButton *button, gpointer user_data) {
    SignupWidgets *app = (SignupWidgets *)user_data;
    GtkWindow *parent_window = GTK_WINDOW(app->window);
    Translations *trans = (app->current_language == LANG_EN) ? &translations_en : &translations_fr;
    
    /* Retrieve text using gtk_editable_get_text */
    const char *name = gtk_editable_get_text(GTK_EDITABLE(app->name_entry));
    const char *surname = gtk_editable_get_text(GTK_EDITABLE(app->surname_entry));
    const char *username = gtk_editable_get_text(GTK_EDITABLE(app->username_entry));
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app->email_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app->password_entry));

    /* Here we check whether each field is not empty, whether the username or email already exists,
     * and whether the password meets requirements.
     */
    if (strlen(name) > 0 && strlen(surname) > 0 && strlen(username) > 0 &&
        strlen(email) > 0 && strlen(password) > 0) {

            // Create the registration message
            char message[500];
            snprintf(message, sizeof(message), "REGISTER|%s|%s|%s|%s|%s", name, surname, username, email, password);

            // Send the registration message to the server
            if (send(sockfd, message, strlen(message), 0) < 0) {
                perror("Send error");
                return;
        }
            // wait for a response from the server
            char response[500];
            int bytes_received = recv(sockfd, response, sizeof(response) - 1, 0);
            if (bytes_received > 0) {
                response[bytes_received] = '\0'; // Null-terminate the response
                if (strcmp(response, "SUCCESS") == 0) {
                    g_print("Registration successful!\n");
                    // Transition to the login screen
                    gtk_stack_set_visible_child_name(GTK_STACK(app->main_stack), "login");
                } else {
                /* Registration failure, create modal message dialog with the invalid_message string */
                GtkWidget *dialog = gtk_message_dialog_new(parent_window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", trans->registration_failed_msg);
                gtk_window_set_transient_for(GTK_WINDOW(dialog), parent_window);
                g_signal_connect(dialog, "response", G_CALLBACK(on_dialog_response), NULL);
                gtk_window_present(GTK_WINDOW(dialog));
                } 
            } else {
                perror("Receive error");
            }
        } else {
            // Show error message for invalid input
            GtkWidget *dialog = gtk_message_dialog_new(parent_window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", trans->invalid_msg);
            gtk_window_set_transient_for(GTK_WINDOW(dialog), parent_window);
            g_signal_connect(dialog, "response", G_CALLBACK(on_dialog_response), NULL);
            gtk_window_present(GTK_WINDOW(dialog));
        }
    }

/* Callback for when the return button (cancel) is clicked */
static void on_return_button_clicked(GtkButton *button, gpointer user_data) {
    SignupWidgets *app = (SignupWidgets *)user_data;
    Translations *trans = (app->current_language == LANG_EN) ? &translations_en : &translations_fr;
    // Cancel registration & transition back to login screen 
    g_print("Return button clicked, going back to the login screen...\n");
    gtk_stack_set_visible_child_name(GTK_STACK(app->main_stack), "login");
}

/* Callback to destroy the dialog when the user responds */
static void on_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data) {
    gtk_window_destroy(GTK_WINDOW(dialog));
}

/* Create the signup page widget.
* The login page UI is built inside a vertical box.
* The "stack" parameter is a pointer to the GtkStack that holds
* all pages. It is stored in the LoginWidgets struct for callbacks.
*/
GtkWidget* create_signup_page(GtkWidget *stack, GtkWidget *window) {

    SignupWidgets *app = malloc(sizeof(SignupWidgets));
    if (!app) {
        g_error("Failed to allocate SignupWidgets");
    }

    // assign the main window pointer
    app->window = window;
    /* Default language set to English */
    app->current_language = LANG_EN;
    /* Save the pointer to the main stack */
    app->main_stack = stack;

    /* Create a top-level vertical box (vbox) to hold sections */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 40);
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
    g_signal_connect(lang_combo, "changed", G_CALLBACK(on_signup_language_changed), app);
    gtk_box_append(GTK_BOX(lang_box), lang_combo);

    /* --- Section 1: Title --- */
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), title_box);
    
    app->register_title_label = gtk_label_new(NULL);
    char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>", translations_en.register_title);
    gtk_label_set_markup(GTK_LABEL(app->register_title_label), markup_title);
    g_free(markup_title);
    gtk_widget_set_halign(app->register_title_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), app->register_title_label);
    
    app->register_subtitle_label = gtk_label_new(translations_en.register_subtitle);
    gtk_widget_set_halign(app->register_subtitle_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), app->register_subtitle_label);

    /* --- Section 2: Registration --- */
    GtkWidget *reg_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), reg_box);

    app->name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->name_entry), translations_en.name_placeholder);
    gtk_widget_set_size_request(app->name_entry, 350, -1);
    gtk_widget_set_hexpand(app->name_entry, FALSE);
    gtk_widget_set_halign(app->name_entry, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(reg_box), app->name_entry);

    app->surname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->surname_entry), translations_en.surname_placeholder);
    gtk_widget_set_size_request(app->surname_entry, 350, -1);
    gtk_widget_set_hexpand(app->surname_entry, FALSE);
    gtk_widget_set_halign(app->surname_entry, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(reg_box), app->surname_entry);

    app->username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->username_entry), translations_en.username_placeholder);
    gtk_widget_set_size_request(app->username_entry, 350, -1);
    gtk_widget_set_hexpand(app->username_entry, FALSE);
    gtk_widget_set_halign(app->username_entry, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(reg_box), app->username_entry);
    
    app->email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->email_entry), translations_en.email_placeholder);
    gtk_widget_set_size_request(app->email_entry, 350, -1);
    gtk_widget_set_hexpand(app->email_entry, FALSE);
    gtk_widget_set_halign(app->email_entry, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(reg_box), app->email_entry);
    
    app->password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(app->password_entry), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->password_entry), translations_en.password_placeholder);
    gtk_widget_set_size_request(app->password_entry, 350, -1);
    gtk_widget_set_hexpand(app->password_entry, FALSE);
    gtk_widget_set_halign(app->password_entry, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(reg_box), app->password_entry);

    app->register_button = gtk_button_new_with_label(translations_en.register_button);
    gtk_widget_set_size_request(app->register_button, 150, -1);
    gtk_widget_set_halign(app->register_button, GTK_ALIGN_CENTER);
    g_signal_connect(app->register_button, "clicked", G_CALLBACK(on_register_button_clicked), app);
    gtk_box_append(GTK_BOX(reg_box), app->register_button);

    /* --- Section 3: Cancel --- */
    GtkWidget *cancel_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(vbox), cancel_box);
    
    app->return_msg = gtk_label_new(translations_en.return_msg);
    gtk_widget_set_halign(app->return_msg, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(cancel_box), app->return_msg);
    
    app->return_button = gtk_button_new_with_label(translations_en.return_button);
    gtk_widget_set_size_request(app->return_button, 150, -1);
    gtk_widget_set_halign(app->return_button, GTK_ALIGN_CENTER);
    g_signal_connect(app->return_button, "clicked", G_CALLBACK(on_return_button_clicked), app);
    gtk_box_append(GTK_BOX(cancel_box), app->return_button);

    gtk_window_present(GTK_WINDOW(app->window));

    return vbox;
}
