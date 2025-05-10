#include <stdio.h>
#include <libpq-fe.h>
#include "create_database.h"
#include "database_setting.h"

/**
 * Creates a new database if it doesn't exist.
 *
 * @param dbname The name of the database to be created.
 * @param conn A pointer to the PostgreSQL connection to be updated.
 *             This connection's context will be switched to the new database.
 * @return The updated connection with the new database context (mydiscord).
 */
PGconn* create_database(const char *dbname, PGconn *conn) {
    // If the database doesn't exist, create it
    char create_db_query[256];
    snprintf(create_db_query, sizeof(create_db_query), "CREATE DATABASE %s;", dbname);

    PGresult *res_create_db = PQexec(conn, create_db_query);

    if (PQresultStatus(res_create_db) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Database creation has failed : %s", PQerrorMessage(conn));
        PQclear(res_create_db);
        PQfinish(conn);
        return NULL;
    }

    printf("Database '%s' creation was successful.\n", dbname);
    PQclear(res_create_db);

    // Switch to the newly created database
    PQfinish(conn);
    
    char conninfo[256];
    snprintf(conninfo,  sizeof(conninfo), "user=%s password=%s host=%s port=%d dbname=%s",
            DB_USER, DB_PASS, DB_HOST, DB_PORT, DB_NAME);
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to the newly created database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
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
        "Status VARCHAR(50) NOT NULL,"
        "PRIMARY KEY(Id_Client),"
        "UNIQUE(User_Name),"
        "UNIQUE(Email)"
    ");";

    PGresult *res_create_table_client = PQexec(conn, table_client);
    
    if (PQresultStatus(res_create_table_client) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Table Client creation has failed : %s", PQerrorMessage(conn));
        PQclear(res_create_table_client);
        PQfinish(conn);
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
    }

    printf("Table Client_Channel creation was successful.\n");
    PQclear(res_create_table_client_channel);

    return conn;
}
