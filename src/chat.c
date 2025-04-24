#include <gtk/gtk.h>
#include <string.h>
#include "dictionaries.h"

/* Structure to hold our UI widgets */
typedef struct {
    GtkWidget *window;
    Language current_language;

    /* UI elements */
    GtkWidget *message_list;    // Container for messages inside the scrollable chat area
    GtkWidget *user_list;       // Container for user info
    GtkWidget *entry;           // Text input for message
    GtkWidget *send_button;     // Send button
    GtkWidget *logout_button;   // Logout button    

    /* New widgets */
    GtkWidget *header_box;      // Contains the large channel name and user info
    GtkWidget *channel_list;    // Container for channel items (left column)
} AppWidgets;

/* Function prototypes */
static void activate(GtkApplication *app, gpointer user_data);
static void update_ui_texts(AppWidgets *app);
static void on_language_changed(GtkComboBoxText *combo, gpointer user_data);
static void on_logout_button_clicked(GtkButton *button, gpointer user_data);

/* CSS styling function */
static void apply_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css_file = "myDiscord.css";

    gtk_css_provider_load_from_path(provider, css_file);
    
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                                 GTK_STYLE_PROVIDER(provider),
                                                 GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

static void
activate(GtkApplication *app, gpointer user_data)
{
    AppWidgets *app_widgets = (AppWidgets *)user_data;

    /* Apply CSS for styling */
    apply_css();

    /* Create the main window */
    app_widgets->window = gtk_application_window_new(app);
    app_widgets->current_language = LANG_EN;
    gtk_window_set_title(GTK_WINDOW(app_widgets->window), "MyDiscord");
    gtk_window_set_default_size(GTK_WINDOW(app_widgets->window), 1000, 700);

    /* Create a top-level grid for full layout */
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_window_set_child(GTK_WINDOW(app_widgets->window), grid);

    // Add a 10-pixel margin on all sides:
    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);
    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);

    /* ==== Top Row: Header with Channel Name, User Info, Language Toggle and Logout Button ==== */

    // Create a header horizontal box that spans all columns.
    app_widgets->header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_hexpand(app_widgets->header_box, TRUE);
    
    // Large Channel Name label:
    GtkWidget *channel_name_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(channel_name_label), "<span size='xx-large'>Main Channel</span>");
    gtk_box_append(GTK_BOX(app_widgets->header_box), channel_name_label);

    // Bold username and status:
    GtkWidget *user_info_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(user_info_label), "<b>MyUsername123 (Online)</b>");
    gtk_widget_set_hexpand(user_info_label, TRUE);
    gtk_box_append(GTK_BOX(app_widgets->header_box), user_info_label);
    
    // Language selector:
    GtkWidget *lang_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "EN");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "FR");
    gtk_combo_box_set_active(GTK_COMBO_BOX(lang_combo), 0);
    g_signal_connect(lang_combo, "changed", G_CALLBACK(on_language_changed), app_widgets);
    gtk_box_append(GTK_BOX(app_widgets->header_box), lang_combo);

    // Logout Button:
    GtkWidget *logout_button = gtk_button_new_with_label(translations_en.logout_button);
    g_signal_connect(logout_button, "clicked", G_CALLBACK(on_logout_button_clicked), app_widgets);
    gtk_box_append(GTK_BOX(app_widgets->header_box), logout_button);

    gtk_window_present(GTK_WINDOW(app_widgets->window));
    
    // Attach the header_box at grid row 0 spanning 10 columns.
    gtk_grid_attach(GTK_GRID(grid), app_widgets->header_box, 0, 0, 10, 1);

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
    app_widgets->channel_list = channel_box;

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
    app_widgets->entry = gtk_entry_new();
    gtk_widget_set_hexpand(app_widgets->entry, TRUE);
    gtk_box_append(GTK_BOX(chat_input_box), app_widgets->entry);
    // Send button:
    app_widgets->send_button = gtk_button_new_with_label("Send");
    gtk_box_append(GTK_BOX(chat_input_box), app_widgets->send_button);

    // Add the chat_input_box at the bottom of the chat_vbox & user vox 
    gtk_grid_attach(GTK_GRID(grid), chat_input_box, 2, 2, 8, 1); 

    // Save the pointer to the chat messages box if needed later:
    app_widgets->message_list = chat_box;

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
}

static void
update_ui_texts(AppWidgets *app)
{
    Translations *trans = NULL;
    if (app->current_language == LANG_EN) {
        trans = &translations_en;
    } else {
        trans = &translations_fr;
    }
    /* Update UI elements with translated strings from trans */
}

static void
on_language_changed(GtkComboBoxText *combo, gpointer user_data)
{
    AppWidgets *app = (AppWidgets *)user_data;
    char *lang = gtk_combo_box_text_get_active_text(combo);
    if (lang) {
        app->current_language = (strcmp(lang, "EN") == 0) ? LANG_EN : LANG_FR;
        update_ui_texts(app);
        g_free(lang);
    }
}

static void
// on_exit_button_clicked(GtkButton *button, gpointer user_data)
on_logout_button_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *app = (AppWidgets *)user_data;
    gtk_window_close(GTK_WINDOW(app->window));
}

int main(int argc, char **argv) {

    GtkApplication *app;
    int status;
    
    /* Create an instance of the AppWidgets and initialize as needed */
    AppWidgets app_widgets = {0};
    
    /* Create a new GtkApplication */
    app = gtk_application_new("com.github.leila-wilde.MyDiscord", 0);
    
    /* Pass the app_widgets as user_data so it is accessible in activate */
    g_signal_connect(app, "activate", G_CALLBACK(activate), &app_widgets);
    
    /* Run the application */
    status = g_application_run(G_APPLICATION(app), argc, argv);
    
    /* Cleanup */
    g_object_unref(app);
    
    return status;
}