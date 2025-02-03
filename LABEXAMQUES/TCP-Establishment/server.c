#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t addrlen = sizeof(cli_addr);
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen the incoming connection
    if (listen(sockfd, 3) < 0)
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER]: Server listening on port %d...\n", PORT);

    

    // Accept the incoming connection
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addrlen);
    if (newsockfd == 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive message
    read(newsockfd, buffer, BUFFER_SIZE);
    printf("[CLIENT]:%s\n", buffer);

    // Send message
    char *message = "HELLO FROM SERVER";
    send(newsockfd, message, strlen(message), 0);

    // Close socket
    close(sockfd);
    close(newsockfd);

    return 0;
}