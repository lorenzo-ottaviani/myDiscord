#include <stdio.h>
#include <libpq-fe.h>
#include "database_setup.h"

int main() {
    // Initial connection to PostgreSQL
    const char *conninfo = "user=postgres password=Nodercna6. host=localhost port=5432 dbname=postgres";
    PGconn *conn = PQconnectdb(conninfo);

    // Check if the connection was successful
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to the database has failed : %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
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
        return 1;
    }

    // If the database exists, we won't proceed to create it
    if (PQntuples(res_check) > 0) {
        PQclear(res_check);
        printf("Database '%s' already exists.\n", dbname);
    } else {
        PQclear(res_check);
        conn = create_database(dbname, conn);
        if (!conn) return 1;
    }

    // Clean up
    PQfinish(conn);

    return 0;
}
