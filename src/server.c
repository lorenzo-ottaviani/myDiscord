#include <sys/types.h>  // Defines data types used in system calls.
#include <sys/socket.h> // Contains definitions of structures needed for sockets; e.g., socket(), bind(), listen(), accept().
#include <netdb.h>      // Provides definitions for network database operations (e.g., getting address information).
#include <fcntl.h>      // Includes flags and function prototypes for file control (e.g., non-blocking I/O).
#include <unistd.h>     // Provides access to the POSIX operating system API (e.g., close(), read(), write()).
#include <stdio.h>      // Standard I/O functions (e.g., printf(), fgets()).
#include <string.h>     // String operations, such as bzero() and strlen().

int main()
{
    char str[100];        // Buffer for receiving data from the client.
    char sendline1[100];  // Buffer for sending data to the client.
    bzero(sendline1, 100); // Initialize send buffer by setting all 100 bytes to 0.
    int listen_fd, comm_fd; // File descriptors for the listening (server) socket and the communication (client connection) socket.
    struct sockaddr_in servaddr; //IPv4_IPv6, IP add, PORT add. Struct used to hold the server's IP address and port number.

int main()
{
 char str[100]; //receiving data
 char sendline1[100];
 bzero(sendline1,100);
 int listen_fd,comm_fd;
 struct sockaddr_in servaddr; 
 
 //creating a socket, address family inet(using v4),use INET6 for v6
 listen_fd=socket(AF_INET,SOCK_STREAM,0);
 //SOCK_STREAM = TCP, SOCK_DGRAM = UDP
 //0 is default protocol not flag operation
 
 
 
 bzero(str,100); //want to make 100 bytes 0
 bzero(&servaddr,sizeof(servaddr)); //initialize all values with 0
 
 //defining server address..passing those 3 parameters
 servaddr.sin_family=AF_INET; //IPv4
 servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 //server will connect with any IP addr
 //htonl is used to convert host byte into network byte
 servaddr.sin_port=htons(21000);
 //0 to 65,535....first 1024 ports are fixed
 
 
 //binding server addr with socket
 bind(listen_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
 //listening 
 listen(listen_fd,5); //5 clients can be connected to the server
 
 //to establish connection with client...like a pipeline
 comm_fd=accept(listen_fd,(struct sockaddr *)NULL,NULL);
 
 //client initializing data
 while(1){
 recv(comm_fd,str,100,0); //hardcoded 100 because we do not know length of incoming stream
 printf("Client: %s",str);
 printf("Your message: ");
 fgets(sendline1,100,stdin);
 send(comm_fd,sendline1,strlen(sendline1),0);
 bzero(sendline1,100);
 //0 is flag operation...tells abt successfull and unsuccessfull op
 }
 close(comm_fd);
}