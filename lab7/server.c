#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BROADCAST_PORT 12345
#define BROADCAST_IP "192.168.1.255"
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in broadcast_addr;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Enable broadcast option
    int broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        perror("Set broadcast option failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Set up the broadcast address structure
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(BROADCAST_PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    printf("Enter messages to broadcast (type 'exit' to quit):\n");
    while (1)
    {
        // Read message from the user
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

        // Exit condition
        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }

        // Send the broadcast message
        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0)
        {
            perror("Broadcast failed");
        }
        else
        {
            printf("Broadcasted: %s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}
