/* dictionaries.h */
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
const char *register_title;
const char *login_subtitle;
const char *register_subtitle;
const char *name_placeholder;
const char *surname_placeholder;
const char *username_placeholder;
const char *email_placeholder;
const char *password_placeholder;
const char *login_button;
const char *logout_button;
const char *invalid_message;
const char *create_account_msg;
const char *return_msg;
const char *register_button;
const char *return_button;
} Translations;

/* Extern declarations for the dictionaries */
extern Translations translations_en;
extern Translations translations_fr;
#endif /* DICTIONARIES_H */