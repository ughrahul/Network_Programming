#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5035
#define BUFFER_SIZE 1024

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serv_adrr, cli_addr;
    socklen_t addrlen = sizeof(cli_addr);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    serv_adrr.sin_family = AF_INET;
    serv_adrr.sin_addr.s_addr = INADDR_ANY;
    serv_adrr.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(sockfd, (struct sockaddr *)&serv_adrr, sizeof(serv_adrr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen to incoming connection
    if (listen(sockfd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER]: Echo server is listening on port %d...", PORT);

    // Accept the incoming connection
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addrlen);
    if (newsockfd == 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data from client
    read(newsockfd, buffer, BUFFER_SIZE);
    printf("[CLIENT]: %s\n\n", buffer);

    // Echo the message back to client
    send(newsockfd, buffer, strlen(buffer), 0);
    printf("[SERVER]: Echoed the message back to the client\n");

    // Close the sockets
    close(newsockfd);
    close(sockfd);
}
