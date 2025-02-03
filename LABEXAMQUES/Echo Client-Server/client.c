#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5035
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 0)
    {
        perror("SOcket Creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Get message from client
    printf("Enter the message:  \n");
    printf("[CLIENT]: ");

    // Read message from client
    fgets(buffer, sizeof(buffer), stdin);

    // Send messaeg to server
    write(sockfd, buffer, strlen(buffer));

    // Receive the echoed message from the server
    read(sockfd, buffer, sizeof(buffer));

    // Display the echo message from the server
    printf("[SERVER]: %s\n", buffer);

    // Close the socket
    close(sockfd);
}