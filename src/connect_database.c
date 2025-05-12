#include <stdio.h>
#include <libpq-fe.h>
#include "create_database.h"
#include "connect_database.h"
#include "database_setting.h"


/**
 * Connect to the PostgreSQL database.
 *
 * @return The connection with the database.
 */
PGconn* connect_database() {
    // Initial connection to PostgreSQL
    char conninfo[256];
    snprintf(conninfo,  sizeof(conninfo), "user=%s password=%s host=%s port=%d dbname=%s",
            DB_USER, DB_PASS, DB_HOST, DB_PORT, DB_NAME);
    PGconn *conn = PQconnectdb(conninfo);

    // Check if the connection was successful
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to the database has failed : %s", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    // Database name
    const char *dbname = "mydiscord";

    // Query to check if the database exists
    char check_db_query[256];
    snprintf(check_db_query, sizeof(check_db_query), 
             "SELECT 1 FROM pg_database WHERE datname = '%s';", dbname);

    // Execute the check query
    PGresult *res_check = PQexec(conn, check_db_query);
 
    if (PQresultStatus(res_check) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Error checking database existence: %s", PQerrorMessage(conn));
        PQclear(res_check);
        PQfinish(conn);
        return NULL;
    }

    // If the database exists, we won't proceed to create it
    if (PQntuples(res_check) > 0) {
        PQclear(res_check);
        printf("Database '%s' already exists.\n", dbname);
    } else {
        PQclear(res_check);
        conn = create_database(dbname, conn);
        if (!conn) return NULL;
    }

    // Return the connection to the database to the main program
    return conn;
}
