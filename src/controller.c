#include "controller.h"
#include "login.h"
#include "signup.h"
#include "chat.h"
#include <string.h>

/* CSS styling function */
void apply_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css_file = "myDiscord.css";

    gtk_css_provider_load_from_path(provider, css_file);
    
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

void update_login_ui_texts(void *widget_struct) {
    LoginWidgets *loginWidget = (LoginWidgets *)widget_struct;
    Translations *trans = (loginWidget->current_language == LANG_EN)
                            ? &translations_en : &translations_fr;
    
    if (loginWidget->login_title_label) {
        /* Create markup for the title (bigger and bolder) */
        char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>",
                                                       trans->login_title);
        gtk_label_set_markup(GTK_LABEL(loginWidget->login_title_label), markup_title);
        g_free(markup_title);
    }
    
    gtk_label_set_text(GTK_LABEL(loginWidget->login_subtitle_label), trans->login_subtitle);
    gtk_label_set_text(GTK_LABEL(loginWidget->create_account_msg), trans->create_account_msg);
    
    gtk_entry_set_placeholder_text(GTK_ENTRY(loginWidget->email_entry), trans->email_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(loginWidget->password_entry), trans->password_placeholder);
    
    gtk_button_set_label(GTK_BUTTON(loginWidget->register_button), trans->register_button);
    gtk_button_set_label(GTK_BUTTON(loginWidget->login_button), trans->login_button);
}

void update_signup_ui_texts(void *widget_struct) {
    SignupWidgets *signupWidget = (SignupWidgets *)widget_struct;
    Translations *trans = (signupWidget->current_language == LANG_EN)
                            ? &translations_en : &translations_fr;
    
    if (signupWidget->register_title_label) {
        /* Create markup for the title (bigger and bolder) */
        char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>",
                                                       trans->register_title);
        gtk_label_set_markup(GTK_LABEL(signupWidget->register_title_label), markup_title);
        g_free(markup_title);
    }
    
    gtk_label_set_text(GTK_LABEL(signupWidget->register_subtitle_label), trans->register_subtitle);
    
    gtk_label_set_text(GTK_LABEL(signupWidget->invalid_msg), trans->invalid_msg);
    gtk_label_set_text(GTK_LABEL(signupWidget->return_msg), trans->return_msg);
    
    gtk_entry_set_placeholder_text(GTK_ENTRY(signupWidget->name_entry), trans->name_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(signupWidget->surname_entry), trans->surname_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(signupWidget->username_entry), trans->username_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(signupWidget->email_entry), trans->email_placeholder);
    gtk_entry_set_placeholder_text(GTK_ENTRY(signupWidget->password_entry), trans->password_placeholder);
    
    gtk_button_set_label(GTK_BUTTON(signupWidget->register_button), trans->register_button);
    gtk_button_set_label(GTK_BUTTON(signupWidget->return_button), trans->return_button);
}

void update_chat_ui_texts(void *widget_struct) {
    ChatWidgets *chatWidget = (ChatWidgets *)widget_struct;
    Translations *trans = (chatWidget->current_language == LANG_EN)
                            ? &translations_en : &translations_fr;
    
    if (chatWidget->main_channel_title_label) {
        /* Create markup for the title (bigger and bolder) */
        char *markup_title = g_markup_printf_escaped("<span weight='bold' size='xx-large'>%s</span>", trans->main_channel_title);
        gtk_label_set_markup(GTK_LABEL(chatWidget->main_channel_title_label), markup_title);
        g_free(markup_title);
    }
    
    gtk_label_set_text(GTK_LABEL(chatWidget->online_status), trans->online_status);
    gtk_label_set_text(GTK_LABEL(chatWidget->offline_status), trans->offline_status);
    gtk_label_set_text(GTK_LABEL(chatWidget->away_status), trans->away_status);

    gtk_label_set_text(GTK_LABEL(chatWidget->channel_list), trans->channel_list_title);
    gtk_label_set_text(GTK_LABEL(chatWidget->user_list), trans->users_list_title);
    
    gtk_entry_set_placeholder_text(GTK_ENTRY(chatWidget->message_entry), trans->message_placeholder);
    
    gtk_button_set_label(GTK_BUTTON(chatWidget->logout_button), trans->logout_button);
    gtk_button_set_label(GTK_BUTTON(chatWidget->send_button), trans->send_button);
}

/* These callbacks are bound to the language combo box in each page. */
void on_login_language_changed(GtkComboBoxText *combo, gpointer user_data) {
    LoginWidgets *app = (LoginWidgets *)user_data;
    const char *lang = gtk_combo_box_text_get_active_text(combo);
    if (strcmp(lang, "EN") == 0)
        app->current_language = LANG_EN;
    else
        app->current_language = LANG_FR;
    
    update_login_ui_texts(app);
}

void on_signup_language_changed(GtkComboBoxText *combo, gpointer user_data) {
    SignupWidgets *app = (SignupWidgets *)user_data;
    const char *lang = gtk_combo_box_text_get_active_text(combo);
    if (strcmp(lang, "EN") == 0)
        app->current_language = LANG_EN;
    else
        app->current_language = LANG_FR;
    
    update_signup_ui_texts(app);
}

void on_chat_language_changed(GtkComboBoxText *combo, gpointer user_data) {
    SignupWidgets *app = (SignupWidgets *)user_data;
    const char *lang = gtk_combo_box_text_get_active_text(combo);
    if (strcmp(lang, "EN") == 0)
        app->current_language = LANG_EN;
    else
        app->current_language = LANG_FR;
    
    update_chat_ui_texts(app);
}
