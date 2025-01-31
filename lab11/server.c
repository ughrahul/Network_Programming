#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Hello from server";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
#ifdef _WIN32
        printf("Socket creation failed: %d\n", WSAGetLastError());
#else
        perror("Socket creation failed");
#endif
        platform_cleanup();
        return 1;
    }

    // Set socket options

#ifdef _WIN32
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)) == -1)
    {
        printf("Setsockopt failed");
        close(server_fd);
        return 1;
    }
#endif

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
#ifdef _WIN32
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
#else
        perror("Bind failed");
        close(server_fd);
#endif
        platform_cleanup();
        return 1;
    }

    // Listen for connections
    if (listen(server_fd, 3) == -1)
    {
#ifdef _WIN32
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
#else
        perror("Listen failed");
        close(server_fd);
#endif
        platform_cleanup();
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    // Accept a client connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) == -1)
    {
#ifdef _WIN32
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
#else
        perror("Accept failed");
        close(server_fd);
#endif
        platform_cleanup();
        return 1;
    }

    // Read data from client
    recv(client_fd, buffer, BUFFER_SIZE, 0);
    printf("Message from client: %s\n", buffer);

    // Send response to client
    send(client_fd, response, strlen(response), 0);
    printf("Response sent to client\n");

// Clean up
#ifdef _WIN32
    closesocket(client_fd);
    closesocket(server_fd);
#else
    close(client_fd);
    close(server_fd);
#endif

    platform_cleanup();
    return 0;
}
