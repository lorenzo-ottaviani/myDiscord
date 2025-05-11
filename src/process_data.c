#include <stdio.h>
#include "signup.h"

#include <stdio.h>
#include <libpq-fe.h>
#include "create_database.h"
#include "connect_database.h"
#include "database_setting.h"
#include "signup_database.h"


void process_signup_form(const char *name, const char *surname, const char *username, const char *email, const char *password) {
    // Exemple simple : envoyer les données au serveur (comme tu le faisais déjà)
    char message[500];
    snprintf(message, sizeof(message), "REGISTER|%s|%s|%s|%s|%s", name, surname, username, email, password);
    printf("name : %s - email : %s\n", name, email);

    signup_database(name, surname, username, email, password);

    connect_database();

}
