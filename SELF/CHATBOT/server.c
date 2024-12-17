#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 5035
#define BUFFER_SIZE 1024

int main()
{
    int sockfd, newsockfd;
    socklen_t clength;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[BUFFER_SIZE];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    serv_addr.sin_port = htons(PORT);

    // Bind the socket to address and port
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Listen for incoming connections (max 5 clients in the queue)
    listen(sockfd, 5);

    printf("Server: Waiting for client connection...\n");

    // Accept a client connection
    clength = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clength);
    printf("Server: Client connected.\n");

    // Chat loop
    while (1)
    {
        // Receive message from client
        memset(buffer, 0, sizeof(buffer)); // Clear buffer
        read(newsockfd, buffer, sizeof(buffer));

        // If client types "exit", close the connection
        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("Server: Ending chat...\n");
            break;
        }

        // Print client message and reply
        printf("Client: %s\n", buffer);
        printf("Server: I'm your chatbot! You said: %s\n", buffer);

        // Send response to client
        write(newsockfd, buffer, strlen(buffer));
    }

    // Close sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}
