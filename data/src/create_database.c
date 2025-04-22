#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

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

    // Check if the database already exists
    if (PQresultStatus(res_check) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Error checking database existence: %s", PQerrorMessage(conn));
        PQclear(res_check);
        PQfinish(conn);
        return 1;
    }

    // If the database exists, we won't proceed to create it
    if (PQntuples(res_check) > 0) {
        printf("Database '%s' already exists.\n", dbname);
    } else {
        // If the database doesn't exist, create it
        char create_db_query[256];
        snprintf(create_db_query, sizeof(create_db_query), "CREATE DATABASE %s;", dbname);

        PGresult *res_create = PQexec(conn, create_db_query);

        // Check if the creation of the database was successful
        if (PQresultStatus(res_create) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Database creation has failed : %s", PQerrorMessage(conn));
            PQclear(res_create);
            PQclear(res_check);
            PQfinish(conn);
            return 1;
        }

        printf("Database '%s' creation was successful.\n", dbname);
        PQclear(res_create);
    }

    // Clean up
    PQclear(res_check);
    PQfinish(conn);

    return 0;
}
