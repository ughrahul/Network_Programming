#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address configuration
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set server IP address

    // Send data to server
    char *message = "Hello from client";
    sendto(client_fd, message, strlen(message), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Message sent to server.\n");

    // Receive response from server
    int n;
    socklen_t addr_len = sizeof(server_addr);
    n = recvfrom(client_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);

    buffer[n] = '\0'; // Null-terminate the received string
    printf("Server: %s\n", buffer);

    // Close the socket
    close(client_fd);

    return 0;
}
