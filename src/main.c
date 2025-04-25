#include <gtk/gtk.h>
#include "dictionaries.h"
#include "controller.h"
#include "login.h"
#include "signup.h"
#include "chat.h"

static void activate(GtkApplication *app_inst, gpointer user_data) {
    // Create the main window.
    GtkWidget *window = gtk_application_window_new(app_inst);
    gtk_window_set_title(GTK_WINDOW(window), "MyDiscord");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);
    
    // Create a GtkStack that will hold multiple pages.
    GtkWidget *stack = gtk_stack_new();
    gtk_widget_set_hexpand(stack, TRUE);
    gtk_widget_set_vexpand(stack, TRUE);
    
    // Create the pages (widgets): login page, signup page, chat page.
    // (Assuming you implement these functions in your respective modules.)
    GtkWidget *login_page = create_login_page();   // from login.c
    GtkWidget *signup_page = create_signup_page();   // from signup.c
    GtkWidget *chat_page = create_chat_page();       // your chat page widget
    
    // Add each page to the stack with a unique name.
    gtk_stack_add_named(GTK_STACK(stack), login_page, "login");
    gtk_stack_add_named(GTK_STACK(stack), signup_page, "signup");
    gtk_stack_add_named(GTK_STACK(stack), chat_page, "chat");
    
    // Optionally, you can add transition animations:
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 300);
    
    // Set the stack as the child of the window.
    gtk_window_set_child(GTK_WINDOW(window), stack);

    /* Set the initial page */
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
    
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.github.leila-wilde.MyDiscord", 0);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
