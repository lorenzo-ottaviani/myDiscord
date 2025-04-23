/* header files */
#include <sys/types.h> // Defines data types used in system calls.
#include <sys/socket.h> // Contains definitions of structures needed for sockets; e.g., socket(), bind(), listen(), accept().
#include <netdb.h> // Provides definitions for network database operations (e.g., getting address information).
#include <fcntl.h> // Includes flags and function prototypes for file control (e.g., non-blocking I/O).
#include <unistd.h> // Provides access to the POSIX operating system API (e.g., close(), read(), write()).
#include <stdio.h> // Standard I/O functions (e.g., printf(), fgets()).
#include <string.h> // String operations, such as bzero() and strlen().

/* Main Function and Variable Declarations */

int main()
{
    char str[100]; // Buffer for receiving data from the client.
    char sendline1[100]; // Buffer for sending data to the client.
    bzero(sendline1, 100); // Initialize send buffer by setting all 100 bytes to 0.
    int listen_fd, comm_fd; // File descriptors for the listening (server) socket and the communication (client connection) socket.
    struct sockaddr_in servaddr; //IPv4_IPv6, IP add, PORT add. Struct used to hold the server's IP address and port number.

/* str and sendline1 are arrays of 100 characters each. 
bzero(sendline1, 100) sets each byte in sendline1 to zero, ensuring it’s an empty string to start.*/

/* Socket Creation */

 //creating a socket, address family inet(using v4),use INET6 for v6
 listen_fd=socket(AF_INET,SOCK_STREAM,0);
 //SOCK_STREAM = TCP, SOCK_DGRAM = UDP
 //0 is default protocol not flag operation
 
 /* Zeroing Buffers and Struct */

 bzero(str,100); //want to make 100 bytes 0 // Initialize receive buffer to zero.
 bzero(&servaddr,sizeof(servaddr)); //initialize all values with 0 // Initialize the server address structure to zero.
 
 /* Defining Server Address*/

 servaddr.sin_family = AF_INET;  // Set the address family to IPv4.
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Allow connections on any IP address of the machine.
//server will connect with any IP addr
 //htonl is used to convert host byte into network byte
 servaddr.sin_port = htons(21000);  // Set the server port to 21000.
 //0 to 65,535....first 1024 ports are fixed
 /* 
    htonl(INADDR_ANY) converts the constant INADDR_ANY (which allows the server to bind to any available interface) from host to network byte order.
    htons(21000) converts the port number to network byte order. Ports range from 0 to 65535; typically, ports below 1024 are reserved.
*/

/* Binding the Socket */
 
 bind(listen_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 /* 
    The bind() function associates the socket (listen_fd) with the specified address and port.
    The cast (struct sockaddr *)&servaddr is used because the bind() function expects a pointer to a generic socket address structure.
*/
 
/* Listening for Connections */
 listen(listen_fd,5); //5 clients can be connected to the server
 /* 
    listen() tells the socket to start listening for incoming connections.
    The second argument, 5, defines the backlog (the maximum number of connections that can be queued while waiting for acceptance).
*/
 
/* Accepting a Client Connection*/
 //to establish connection with client...like a pipeline
 comm_fd=accept(listen_fd,(struct sockaddr *)NULL,NULL);
 /* 
    accept() blocks until a client connects.
    It returns a new file descriptor (comm_fd) for the newly accepted connection.
    The second and third parameters are used if you want to get the client's address information; here, they are passed as NULL because that information is not needed.
*/
 
/* Communication Loop */
 //client initializing data
 while(1){
 recv(comm_fd,str,100,0); //hardcoded 100 because we do not know length of incoming stream
 printf("Client: %s",str);
 printf("Your message: ");
 fgets(sendline1,100,stdin);
 send(comm_fd,sendline1,strlen(sendline1),0);
 bzero(sendline1,100);
 //0 is flag operation...tells abt successfull and unsuccessfull op
 /* 
    The recv() function reads data from the client connection into the buffer str.
    The server then prints the received message.
    It prompts the operator for a message via standard input using fgets().
    The send() function sends the operator's message to the client.
    bzero() is used after each send to clear the buffer so it does not contain leftover data.
*/
 }

 /* Closing the Connection */
 close(comm_fd);
}