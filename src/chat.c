#include <gtk/gtk.h>
#include <string.h>
#include "chat.h"
#include "dictionaries.h"
#include "controller.h"


static void on_send_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Message sent!\n");
    // encrypt and send message to server
    // display message in chat window
}

/* Callback to destroy the dialog when the user responds */
static void on_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data) {
    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void on_logout_button_clicked(GtkButton *button, gpointer user_data) {
    ChatWidgets *app = (ChatWidgets *)user_data;
    g_print("Logout button clicked! going back to the login screen...\n");
    // impliment logout functions; signout
    // Transistion to login page
    gtk_stack_set_visible_child_name(GTK_STACK(app->main_stack), "login");
}

static void on_status_changed(GtkComboBoxText *combo, gpointer user_data) {
    ChatWidgets *app = (ChatWidgets *)user_data;
    char *status = gtk_combo_box_text_get_active_text(combo);
        if (status) {
        // Extract the current username from the label.
        // In this simple case, we assume the username is fixed.
        const char *username = "MyUsername123";

            // Create a new markup string combining the username with the new status.
            // You could add translations here if needed.
            char markup[256];
            snprintf(markup, sizeof(markup), "<b>%s (%s)</b>", username, status);
            gtk_label_set_markup(GTK_LABEL(app->user_info_label), markup);
            
            g_free(status);
        }
}

GtkWidget* create_chat_page(GtkWidget *stack, GtkWidget *window) {

    ChatWidgets *app = malloc(sizeof(ChatWidgets));
    if (!app) {
        g_error("Failed to allocate memory for ChatWidgets");
    }

    // assign the main window pointer
    app->window = window;
    /* Default language set to English */
    app->current_language = LANG_EN;
    /* Save the pointer to the main stack */
    app->main_stack = stack;

    /* Create a top-level grid for full layout */
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_window_set_child(GTK_WINDOW(app->window), grid);

    // Add a 10-pixel margin on all sides:
    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);
    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);

    /* ==== Top Row: Header with Channel Name, User Info, Language Toggle and Logout Button ==== */

    // Create a header horizontal box that spans all columns.
    app->header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_hexpand(app->header_box, TRUE);
    
    // Large Channel Name label:
    GtkWidget *channel_name_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(channel_name_label), "<span size='xx-large'>Main Channel</span>");
    gtk_box_append(GTK_BOX(app->header_box), channel_name_label);

    // Bold username with status.
    // Instead of a local variable, we save the pointer inside AppWidgets so we can update it later.
    app->user_info_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(app->user_info_label), "<b>MyUsername123</b>");
    gtk_widget_set_hexpand(app->user_info_label, TRUE);
    gtk_box_append(GTK_BOX(app->header_box), app->user_info_label);

    // Status selector:
    GtkWidget *status_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(status_combo), "Online");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(status_combo), "Away");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(status_combo), "Offline");
    gtk_combo_box_set_active(GTK_COMBO_BOX(status_combo), 0);
    g_signal_connect(status_combo, "changed", G_CALLBACK(on_status_changed), app);
    gtk_box_append(GTK_BOX(app->header_box), status_combo);
  
    // Language selector:
    GtkWidget *lang_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "EN");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "FR");
    gtk_combo_box_set_active(GTK_COMBO_BOX(lang_combo), 0);
    g_signal_connect(lang_combo, "changed", G_CALLBACK(on_chat_language_changed), app);
    gtk_box_append(GTK_BOX(app->header_box), lang_combo);

    // Logout Button:
    GtkWidget *logout_button = gtk_button_new_with_label(translations_en.logout_button);
    g_signal_connect(logout_button, "clicked", G_CALLBACK(on_logout_button_clicked), app);
    gtk_box_append(GTK_BOX(app->header_box), logout_button);

    gtk_window_present(GTK_WINDOW(app->window));
    
    // Attach the header_box at grid row 0 spanning 10 columns.
    gtk_grid_attach(GTK_GRID(grid), app->header_box, 0, 0, 10, 1);

    /* ==== Middle Area: 10 Columns in the Grid ==== */
    /* layout:
         Column 0-1: Channel List 
         Columns 2-7: Chat Area (wider)
         Column 8-9: User List
    */

    // ----- Left Column: Channel List (scrollable)
    GtkWidget *channel_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    // Example channels:
    gtk_box_append(GTK_BOX(channel_box), gtk_label_new("General"));
    gtk_box_append(GTK_BOX(channel_box), gtk_label_new("Random"));
    gtk_box_append(GTK_BOX(channel_box), gtk_label_new("Tech News"));
    gtk_box_append(GTK_BOX(channel_box), gtk_label_new("Gaming"));
    gtk_box_append(GTK_BOX(channel_box), gtk_label_new("Music"));
    // More channels can be added here

    // Make the channel list scrollable
    GtkWidget *channel_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(channel_scrolled), channel_box);
    gtk_widget_set_vexpand(channel_scrolled, TRUE);
    gtk_widget_set_hexpand(channel_scrolled, TRUE);
    // Wrap into a frame with a title (optional)
    GtkWidget *channel_frame = gtk_frame_new("Channels");
    gtk_frame_set_child(GTK_FRAME(channel_frame), channel_scrolled);
    // Save pointer for later use if needed
    app->channel_list = channel_box;

    // Attach left column 
    gtk_grid_attach(GTK_GRID(grid), channel_frame, 0, 1, 2, 2);

    // ----- Middle Column: Chat Window (chat area container)
    GtkWidget *chat_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_vexpand(chat_vbox, TRUE);
    gtk_widget_set_hexpand(chat_vbox, TRUE);

    // Create the chat messages box (scrollable):
    GtkWidget *chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    // Sample chat messages:
    GtkWidget *message1 = gtk_label_new("User1: Hello! [10:30 AM]");
    GtkWidget *message2 = gtk_label_new("Me: Hi there! [10:31 AM]");
    gtk_widget_set_halign(message1, GTK_ALIGN_START);
    gtk_widget_set_halign(message2, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(chat_box), message1);
    gtk_box_append(GTK_BOX(chat_box), message2);

    // Make the chat messages area scrollable:
    GtkWidget *chat_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chat_scrolled), chat_box);
    gtk_widget_set_vexpand(chat_scrolled, TRUE);
    gtk_widget_set_hexpand(chat_scrolled, TRUE);
    // Optionally wrap in a frame:
    GtkWidget *chat_frame = gtk_frame_new(NULL);
    gtk_frame_set_child(GTK_FRAME(chat_frame), chat_scrolled);

    // Add the chat_frame to the top of the chat_vbox
    gtk_box_append(GTK_BOX(chat_vbox), chat_frame);

    // Create the input area (text entry and send button) for the chat area:
    GtkWidget *chat_input_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    // Text entry:
    app->message_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->message_entry), translations_en.message_placeholder);
    gtk_widget_set_hexpand(app->message_entry, TRUE);
    gtk_box_append(GTK_BOX(chat_input_box), app->message_entry);
    
    // Send button:
    app->send_button = gtk_button_new_with_label("Send");
    gtk_box_append(GTK_BOX(chat_input_box), app->send_button);

    // Add the chat_input_box at the bottom of the chat_vbox & user vox 
    gtk_grid_attach(GTK_GRID(grid), chat_input_box, 2, 2, 8, 1); 

    // Save the pointer to the chat messages box if needed later:
    app->message_list = chat_box;

    // Attach the chat_vbox to the grid in the middle columns
    gtk_grid_attach(GTK_GRID(grid), chat_vbox, 2, 1, 6, 1);

    // ----- Right Column: User List (scrollable)

    GtkWidget *user_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    // Example online users:
    gtk_box_append(GTK_BOX(user_box), gtk_label_new("User1 (Online)"));
    gtk_box_append(GTK_BOX(user_box), gtk_label_new("User2 (Offline)"));
    gtk_box_append(GTK_BOX(user_box), gtk_label_new("User3 (Online)"));

    // Make the user list scrollable:
    GtkWidget *user_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(user_scrolled), user_box);
    gtk_widget_set_vexpand(user_scrolled, TRUE);
    gtk_widget_set_hexpand(user_scrolled, TRUE);
    // Wrap into a frame titled "Users":
    GtkWidget *user_frame = gtk_frame_new("Users");
    gtk_frame_set_child(GTK_FRAME(user_frame), user_scrolled);

    // Attach right column (column 9-10, row 1)
    gtk_grid_attach(GTK_GRID(grid), user_frame, 8, 1, 2, 1);

    return grid;
}
