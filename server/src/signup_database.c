#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>
#include <sodium.h>
#include "signup_database.h"
#include "database_setting.h"

void signup_database(const char *name, const char *surname, const char *username, const char *email, const char *password) {
    // Initialize libsodium library
    if (sodium_init() < 0) {
        fprintf(stderr, "Échec de l'initialisation de libsodium\n");
        return;
    }

    // Hash the password
    char hashed_password[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(
            hashed_password,
            password,
            strlen(password),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        fprintf(stderr, "Erreur de hachage du mot de passe\n");
        return;
    }

    // Connection string
    char conninfo[256];
    snprintf(conninfo, sizeof(conninfo), "user=%s password=%s host=%s port=%d dbname=%s",
             DB_USER, DB_PASS, DB_HOST, DB_PORT, DB_NAME);
    PGconn *conn = PQconnectdb(conninfo);

    // Check connection status
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return;
    }

    // Prepare SQL query
    const char *query = "INSERT INTO Client (First_Name, Last_Name, User_Name, Email, Password, Role, Status) "
                        "VALUES ($1, $2, $3, $4, $5, 'user', 'inactive')";
    const char *params[5] = { name, surname, username, email, hashed_password };

    // Execute query
    PGresult *res = PQexecParams(conn, query, 5, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Insert failed: %s", PQerrorMessage(conn));
    } else {
        printf("User registered successfully.\n");
    }

    // Clean up
    PQclear(res);
    PQfinish(conn);
}