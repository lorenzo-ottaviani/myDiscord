#ifndef DATABASE_SETUP_H
#define DATABASE_SETUP_H

#include <libpq-fe.h>

PGconn* create_database(const char *dbname, PGconn *conn);

#endif // DATABASE_SETUP_H