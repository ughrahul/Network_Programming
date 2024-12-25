#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define CLI_PORT 5035
#define BUFFER_SIZE 1024

int main()
{
    int client_fd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};

    // Create the socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CLI_PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    printf("Connected to the server\n");

    // Step 1: Send SYN
    send(client_fd, "SYN", strlen("SYN"), 0);
    printf("Sent: SYN\n");

    // Step 2: Receive SYN-ACK
    read(client_fd, buffer, BUFFER_SIZE);
    if (strcmp(buffer, "SYN-ACK") == 0)
    {
        printf("Received: SYN-ACK\n");

        // Step 3: Send ACK
        send(client_fd, "ACK", strlen("ACK"), 0);
        printf("Sent: ACK\n");
    }

    // Close the socket
    close(client_fd);
    return 0;
}
