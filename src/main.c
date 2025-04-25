#include <gtk/gtk.h>
#include "controller.h"
#include "login.h"
#include "signup.h"
#include "chat.h"

static void activate(GtkApplication *app, gpointer user_data) {

    // Allocate our context structure
    AppWidgets *app_ctr = g_malloc(sizeof(AppWidgets));

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MyDiscord");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);

    /* Create a GtkStack */
    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_window_set_child(GTK_WINDOW(window), stack);

    /* Create individual pages */
    GtkWidget *login_page = create_login_page();   // Build login page
    GtkWidget *signup_page = create_signup_page();   // Build signup page
    GtkWidget *chat_page = create_chat_page();           // Build main page

    /* Add pages to the stack with unique names */
    gtk_stack_add_named(GTK_STACK(stack), login_page, "login");
    gtk_stack_add_named(GTK_STACK(stack), signup_page, "signup");
    gtk_stack_add_named(GTK_STACK(stack), chat_page, "chat");

    /* Set the initial page */
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");

    gtk_widget_show(window);

}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.github.leila-wilde.MyDiscord", 0);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}






























/* Callback to switch screens */
static void show_register_screen(GtkButton *button, gpointer user_data) {
GtkStack *stack = GTK_STACK(user_data);
gtk_stack_set_visible_child_name(stack, "register");
}

/* Similarly, switch from register to login or chatroom... */

int main (int argc, char **argv) {
GtkApplication *app = gtk_application_new("com.example.MyDiscord", 0);
g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
int status = g_application_run(G_APPLICATION(app), argc, argv);
g_object_unref(app);
return status;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MyDiscord");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);
    /* Create a stack */
    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_window_set_child(GTK_WINDOW(window), stack);

    /* Create login, registration, chatroom widgets */
    GtkWidget *login_page = create_login_page(stack);       // Design a function that returns a pre-built widget container
    GtkWidget *register_page = create_register_page(stack);
    GtkWidget *chatroom_page = create_chatroom_page(stack);

    /* Add pages to the stack - assign each a unique name */
    gtk_stack_add_named(GTK_STACK(stack), login_page, "login");
    gtk_stack_add_named(GTK_STACK(stack), register_page, "register");
    gtk_stack_add_named(GTK_STACK(stack), chatroom_page, "chat");

    /* Initially show the login page */
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");

    gtk_widget_show(window);

}

// Within the login button callback, once authentication succeeds, switch to the chatroom page:

static void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    AppWidgets app = (AppWidgets) user_data;
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app->email_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app->password_entry));
        if (is_valid_credentials(email, password)) {
            /* if login successful, switch page */
            gtk_stack_set_visible_child_name(GTK_STACK(app->main_stack), "chat");
        } else {
            /* show error dialog */
        }
}
