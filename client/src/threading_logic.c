/* SERVER.C */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 21000
#define BUFFER_SIZE 100

// This function will run as a separate thread to handle client communication.
void *handle_client(void *arg) {
    int comm_fd = *(int *)arg;
    free(arg);  // free the allocated memory for descriptor
    char buffer[BUFFER_SIZE];
    
    // Clear the buffer initially.
    bzero(buffer, BUFFER_SIZE);
    
    // Communicate with the client.
    while (1) {
        int bytes_received = recv(comm_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            // Client closed the connection or an error occurred.
            printf("Client disconnected or error occurred\n");
            break;
        }
        printf("Client: %s", buffer);

        buffer[bytes_received] = '\0'; // Null-terminate the received string

        // Check if the message is a registration request
        if (strncmp(buffer, "REGISTER", 8) == 0) {
            // Parse the registration details
            char *name = strtok(NULL, "|");
            char *surname = strtok(NULL, "|");
            char *username = strtok(buffer + 9, "|");
            char *email = strtok(NULL, "|");
            char *password = strtok(NULL, "|");
        
            printf("Received registration details:\n");
            printf("Name: %s\n", name);
            printf("Surname: %s\n", surname);
            printf("Username: %s\n", username);
            printf("Email: %s\n", email);
            printf("Password: %s\n", password);
        
        // Send a success response back to the client
        const char *response = "SUCCESS";
        send(comm_fd, response, strlen(response), 0);
    } else {
        // Handle other messages (e.g., chat messages)
        printf("Received message: %s\n", buffer);
        // Echo back the message for demonstration
        send(comm_fd, buffer, bytes_received, 0);
    }
    // Clear buffer for the next iteration.
    bzero(buffer, BUFFER_SIZE);
}
        
    close(comm_fd);
    return NULL;
}

int main() {
    int listen_fd;
    struct sockaddr_in servaddr;
    
    // Create the listening socket.
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    
    // Initialize servaddr.
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    // Bind the socket to the address.
    if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections.
    if (listen(listen_fd, 5) < 0) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);
    
    while (1) {
        struct sockaddr_in clientaddr;
        socklen_t addr_len = sizeof(clientaddr);
        
        // Accept a new connection.
        int *comm_fd = malloc(sizeof(int));
        if ((*comm_fd = accept(listen_fd, (struct sockaddr *)&clientaddr, &addr_len)) < 0) {
            perror("Accept error");
            free(comm_fd);
            continue;
        }
        
        printf("Accepted a connection...\n");
        
        // Create a new thread to handle the client.
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, comm_fd) != 0) {
            perror("Thread creation error");
            close(*comm_fd);
            free(comm_fd);
            continue;
        }
        
        // Optionally, detach the thread so its resources are freed when it finishes.
        pthread_detach(tid);
    }
    
    close(listen_fd);
    return 0;
}



/* CLIENT.C */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 21000
#define BUFFER_SIZE 100

// Global socket file descriptor so that it can be accessed in both threads.
int sockfd;

// This thread function will continuously receive messages from the server.
void *receive_messages(void *arg) {
    char recvline[BUFFER_SIZE];
    while (1) {
        bzero(recvline, BUFFER_SIZE);
        int bytes_received = recv(sockfd, recvline, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Server closed connection or error occurred.\n");
            break;
        }
        printf("Server: %s", recvline);
    }
    exit(0);  // End program if server disconnects.
    return NULL;
}

int main() {
    struct sockaddr_in servaddr;
    
    // Create socket.
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    
    // Initialize server address.
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Connect to the server.
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connect error");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server at 127.0.0.1:%d\n", PORT);
    
    // Create a thread to handle receiving messages.
    pthread_t rcv_tid;
    if (pthread_create(&rcv_tid, NULL, receive_messages, NULL) != 0) {
        perror("Thread creation error");
        exit(EXIT_FAILURE);
    }
    
    // Main thread handles sending messages.
    char sendline[BUFFER_SIZE];
    while (1) {
        bzero(sendline, BUFFER_SIZE);
        printf("Your Message: ");
        fgets(sendline, BUFFER_SIZE, stdin);
        if (send(sockfd, sendline, strlen(sendline), 0) < 0) {
            perror("Send error");
            break;
        }
    }
    
    close(sockfd);
    return 0;
}
