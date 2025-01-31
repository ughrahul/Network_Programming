#include <stdio.h>
#include <winsock2.h> // Include Winsock header

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;     // Structure to store Winsock information
    SOCKET serverSocket; // Socket descriptor
    struct sockaddr_in serverAddr, clientAddr;
    int addrlen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE] = {0};
    const char *response = "Hello from Server";

    // Step 1: Initialize Winsock with WSAStartup()
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    // Step 2: Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        printf("Socket creation failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Step 3: Set up the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Step 4: Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Step 5: Listen for connections
    if (listen(serverSocket, 3) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Step 6: Accept a client connection
    SOCKET clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrlen);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Step 7: Receive data from client
    recv(clientSocket, buffer, BUFFER_SIZE, 0);
    printf("Message from client: %s\n", buffer); // Fixed missing format specifier

    // Step 8: Send response to client
    send(clientSocket, response, strlen(response), 0);
    printf("Response sent to client\n");

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
