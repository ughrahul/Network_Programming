#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5035
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost (can be changed to a remote server's IP)
    serv_addr.sin_port = htons(PORT);

    // Connect to the server
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Client: Connected to server.\n");

    // Chat loop
    while (1)
    {
        // Get user input
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);

        // If user types "exit", break out of the loop
        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("Client: Exiting...\n");
            break;
        }

        // Send message to server
        write(sockfd, buffer, strlen(buffer));

        // Receive server response
        memset(buffer, 0, sizeof(buffer)); // Clear buffer
        read(sockfd, buffer, sizeof(buffer));
        printf("Server: %s\n", buffer);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
