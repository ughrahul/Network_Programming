/*write a program using TCP/IP to implement three way handshaking
protocol for a socket programming */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5035
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};
    socklen_t addr_len = sizeof(address);

    // Create the socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_fd, 3);

    printf("Server is listening on port %d...\n", PORT);

    // Accept a connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);
    printf("Connection established with client\n");

    // Step 1: Receive SYN
    read(new_socket, buffer, BUFFER_SIZE);
    if (strcmp(buffer, "SYN") == 0)
    {
        printf("Received: SYN\n");

        // Step 2: Send SYN-ACK
        send(new_socket, "SYN-ACK", strlen("SYN-ACK"), 0);
        printf("Sent: SYN-ACK\n");

        // Step 3: Receive ACK
        memset(buffer, 0, BUFFER_SIZE);
        read(new_socket, buffer, BUFFER_SIZE);
        if (strcmp(buffer, "ACK") == 0)
        {
            printf("Received: ACK\n");
            printf("Three-way handshake completed successfully!\n");
        }
    }

    // Close the socket
    close(new_socket);
    close(server_fd);
    return 0;
}
