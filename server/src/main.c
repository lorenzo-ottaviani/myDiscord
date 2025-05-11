// Include server libraries for each OS
#if defined(_WIN32)
    #include <winsock2.h> 
    #include <ws2tcpip.h>
    #include "server_windows.h"

#elif defined(__linux__)
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include "server_linux.h"

#endif

// Others inclusions of libraries and headers file
#include <libpq-fe.h>
#include "connect_database.h"

int main() {

    // First connection to the database
    PGconn *conn = connect_database();

    // Server creation
    #if defined(_WIN32)
        init_server_windows();
    #elif defined(__linux__)
        init_server_linux();
    #endif

    // Clean up
    PQfinish(conn);

    return 0;
}
