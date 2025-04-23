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

        PGresult *res_create_db = PQexec(conn, create_db_query);

        // Check if the creation of the database was successful
        if (PQresultStatus(res_create_db) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Database creation has failed : %s", PQerrorMessage(conn));
            PQclear(res_create_db);
            PQclear(res_check);
            PQfinish(conn);
            return 1;
        }

        printf("Database '%s' creation was successful.\n", dbname);
        PQclear(res_create_db);

        // Switch to the newly created database
        PQfinish(conn);
        conn = PQconnectdb("user=postgres password=Nodercna6. host=localhost port=5432 dbname=mydiscord");

        if (PQstatus(conn) != CONNECTION_OK) {
            fprintf(stderr, "Connection to the newly created database failed: %s", PQerrorMessage(conn));
            PQfinish(conn);
            return 1;
        }

        // Create table Client
        const char *table_client =
        "CREATE TABLE Client("
            "Id_Client SERIAL,"
            "First_Name VARCHAR(50) NOT NULL,"
            "Last_Name VARCHAR(50) NOT NULL,"
            "User_Name VARCHAR(50) NOT NULL,"
            "Email VARCHAR(255) NOT NULL,"
            "Password VARCHAR(255) NOT NULL,"
            "Role VARCHAR(50) NOT NULL,"
            "PRIMARY KEY(Id_Client),"
            "UNIQUE(User_Name),"
            "UNIQUE(Email)"
        ");";

        PGresult *res_create_table_client = PQexec(conn, table_client);
        
        if (PQresultStatus(res_create_table_client) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Table Client creation has failed : %s", PQerrorMessage(conn));
            PQclear(res_create_table_client);
            PQfinish(conn);
            return 1;
        }
        printf("Table Client creation was successful.\n");
        PQclear(res_create_table_client);

        // Create table Message
        const char *table_message =
        "CREATE TABLE Message("
            "Id_Message SERIAL,"
            "User_Name VARCHAR(255) NOT NULL,"
            "Content TEXT NOT NULL,"
            "Time_ TIMESTAMP NOT NULL,"
            "Channel VARCHAR(50) NOT NULL,"
            "Id_Client INTEGER NOT NULL,"
            "PRIMARY KEY(Id_Message),"
            "FOREIGN KEY(Id_Client) REFERENCES Client(Id_Client)"
        ");";

        PGresult *res_create_table_message = PQexec(conn, table_message);
        
        if (PQresultStatus(res_create_table_message) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Table Message creation has failed : %s", PQerrorMessage(conn));
            PQclear(res_create_table_message);
            PQfinish(conn);
            return 1;
        }

        printf("Table Message creation was successful.\n");
        PQclear(res_create_table_message);

        // Create table Channel
        const char *table_channel =
        "CREATE TABLE Channel("
            "Id_Channel SERIAL,"
            "Name VARCHAR(50) NOT NULL,"
            "Type VARCHAR(50) NOT NULL,"
            "PRIMARY KEY(Id_Channel),"
            "UNIQUE(Name)"
        ");";

        PGresult *res_create_table_channel = PQexec(conn, table_channel);
        
        if (PQresultStatus(res_create_table_channel) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Table Channel creation has failed : %s", PQerrorMessage(conn));
            PQclear(res_create_table_channel);
            PQfinish(conn);
            return 1;
        }

        printf("Table Channel creation was successful.\n");
        PQclear(res_create_table_channel);

        // Create table Reactions
        const char *table_reactions =
        "CREATE TABLE Reactions("
            "Id_Reactions SERIAL,"
            "User_Name VARCHAR(50) NOT NULL,"
            "Type VARCHAR(50) NOT NULL,"
            "Id_Message INTEGER NOT NULL,"
            "PRIMARY KEY(Id_Reactions),"
            "FOREIGN KEY(Id_Message) REFERENCES Message(Id_Message)"
        ");";

        PGresult *res_create_table_reactions = PQexec(conn, table_reactions);
        
        if (PQresultStatus(res_create_table_reactions) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Table Reactions creation has failed : %s", PQerrorMessage(conn));
            PQclear(res_create_table_reactions);
            PQfinish(conn);
            return 1;
        }

        printf("Table Reactions creation was successful.\n");
        PQclear(res_create_table_reactions);

        // Create table Client_Channel (junction table)
        const char *table_client_channel =
        "CREATE TABLE Client_Channel("
            "Id_Client INTEGER,"
            "Id_Channel INTEGER,"
            "PRIMARY KEY(Id_Client, Id_Channel),"
            "FOREIGN KEY(Id_Client) REFERENCES Client(Id_Client),"
            "FOREIGN KEY(Id_Channel) REFERENCES Channel(Id_Channel)"
        ");";
        
        PGresult *res_create_table_client_channel = PQexec(conn, table_client_channel);
        
        if (PQresultStatus(res_create_table_client_channel) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Table Client_Channel creation has failed : %s", PQerrorMessage(conn));
            PQclear(res_create_table_client_channel);
            PQfinish(conn);
            return 1;
        }

        printf("Table Client_Channel creation was successful.\n");
        PQclear(res_create_table_client_channel);
    }

    // Clean up
    PQclear(res_check);
    PQfinish(conn);

    return 0;
}
