#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address configuration
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is running on port %d\n", PORT);

    // Receive data from client
    int n = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
    buffer[n] = '\0'; // Null-terminate the received string
    printf("Client: %s\n", buffer);

    // Send response to client
    char *response = "Hello from server";
    sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
    printf("Response sent to client.\n");

    // Close the socket
    close(server_fd);

    return 0;
}
