#ifndef CREATE_DATABASE_H
#define CREATE_DATABASE_H

#include <libpq-fe.h>

PGconn* create_database(const char *dbname, PGconn *conn);

#endif
