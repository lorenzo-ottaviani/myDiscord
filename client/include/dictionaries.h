#ifndef DICTIONARIES_H
#define DICTIONARIES_H

/* Definition of the supported languages */
typedef enum {
LANG_EN,
LANG_FR
} Language;

/* Structure for the translation texts */
typedef struct {
const char *login_title;
const char *login_subtitle;
const char *register_title;
const char *register_subtitle;
const char *main_channel_title;

const char *online_status;
const char *offline_status;
const char *away_status;

const char *name_placeholder;
const char *surname_placeholder;
const char *username_placeholder;
const char *email_placeholder;
const char *password_placeholder;
const char *message_placeholder;

const char *channel_list_title;
const char *users_list_title;

const char *send_button;
const char *login_button;
const char *logout_button;
const char *register_button;
const char *return_button;

const char *invalid_msg;
const char *create_account_msg;
const char *return_msg;

} Translations;

/* Extern declarations for the dictionaries */
extern Translations translations_en;
extern Translations translations_fr;

#endif 