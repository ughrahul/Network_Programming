#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include platform-specific headers
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Link Winsock library
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#define SERVER_IP "127.0.0.1" // Server address (localhost)
#define PORT 8080
#define BUFFER_SIZE 1024

// Platform-specific cleanup function
void platform_cleanup()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

int main()
{
#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }
#endif

    int client_fd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hello from client";

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
#ifdef _WIN32
        printf("Socket creation failed: %d\n", WSAGetLastError());
#else
        perror("Socket creation failed");
#endif
        platform_cleanup();
        return 1;
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
    {
        printf("Invalid address / Address not supported\n");
        platform_cleanup();
        return 1;
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
#ifdef _WIN32
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(client_fd);
#else
        perror("Connection failed");
        close(client_fd);
#endif
        platform_cleanup();
        return 1;
    }

    // Send message to server
    send(client_fd, message, strlen(message), 0);
    printf("Message sent to server\n");

    // Receive response from server
    recv(client_fd, buffer, BUFFER_SIZE, 0);
    printf("Server response: %s\n", buffer);

// Clean up
#ifdef _WIN32
    closesocket(client_fd);
#else
    close(client_fd);
#endif

    platform_cleanup();
    return 0;
}
