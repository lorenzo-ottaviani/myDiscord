#include <gtk/gtk.h>
#include "dictionaries.h"
#include "controller.h"
#include "login.h"
#include "signup.h"
#include "chat.h"

// Global variable for the socket file descriptor
extern int sockfd;

static void activate(GtkApplication *app_inst, gpointer user_data) {
    // Apply CSS before creating widgets
    apply_css();
        
    // Create the main window.
    GtkWidget *window = gtk_application_window_new(app_inst);
    gtk_window_set_title(GTK_WINDOW(window), "MyDiscord");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 700);
    
    // Create a GtkStack that will hold multiple pages.
    GtkWidget *stack = gtk_stack_new();
    gtk_widget_set_hexpand(stack, TRUE);
    gtk_widget_set_vexpand(stack, TRUE);
    
    // Create the pages (as widgets): login page, signup/register page, chat page.
    GtkWidget *login_page = create_login_page(stack, window);
    GtkWidget *signup_page = create_signup_page(stack, window);
    GtkWidget *chat_page = create_chat_page(stack, window);
    
    // Add each page to the stack with a unique name.
    gtk_stack_add_named(GTK_STACK(stack), login_page, "login");
    gtk_stack_add_named(GTK_STACK(stack), signup_page, "signup");
    gtk_stack_add_named(GTK_STACK(stack), chat_page, "chat");
    
    // Add transition animations:
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_ROTATE_LEFT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 300);
    
    // Set the stack as the child of the window.
    gtk_window_set_child(GTK_WINDOW(window), stack);

    // Set the initial page.
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
    
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {

    GtkApplication *app;
    int status;
    
    /* Create a new GtkApplication */
    app = gtk_application_new("com.github.leila-wilde.MyDiscord", 0);
    
    /* Pass the app_widgets as user_data so it is accessible in activate */
    g_signal_connect(app, "activate", G_CALLBACK(activate), app);
    
    /* Run the application */
    status = g_application_run(G_APPLICATION(app), argc, argv);

    
    
    /* Cleanup */
    g_object_unref(app);
    
    return status;
}