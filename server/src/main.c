#include <stdio.h>
#include <libpq-fe.h>
#include "connect_database.h"

int main() {

    // First connection to the database
    PGconn *conn = connect_database();

    // Clean up
    PQfinish(conn);

    return 0;
}
