#ifndef DATABASE_SETUP_H
#define DATABASE_SETUP_H

#include <libpq-fe.h>

PGconn* create_database(const char *dbname, PGconn *conn);

void signup_database(const char *name, const char *surname, const char *username, const char *email, const char *password);

#ifdef _WIN32
    #define BCRYPT_HASHSIZE 60
    extern char hashed_password[BCRYPT_HASHSIZE];
#endif

#endif