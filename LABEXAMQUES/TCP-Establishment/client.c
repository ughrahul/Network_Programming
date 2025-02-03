#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 0)
    {
        perror("Socket Creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)

    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send message
    char *message = " Hello from Client";
    send(sockfd, message, strlen(message), 0);

    // Receive message
    read(sockfd, buffer, BUFFER_SIZE);
    printf("[SERVER]:%s\n", buffer);

    // Close socket
    close(sockfd);
}
