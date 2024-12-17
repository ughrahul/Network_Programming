#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define SERV_TCP_PORT 5035 // Server TCP port number

int main()
{
    int sockfd, newsockfd;
    socklen_t clength;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[4096];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    printf("Starting server...\n");
    fflush(stdout); // Ensure this is printed immediately

    // Bind the socket to the address and port
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Listening for connections...\n");
    fflush(stdout);

    // Listen for incoming connections
    listen(sockfd, 5);

    // Accept a client connection
    clength = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clength);
    printf("Connection accepted.\n");
    fflush(stdout);

    // Read data from the client
    read(newsockfd, buffer, sizeof(buffer));
    printf("Client message: %s\n", buffer);
    fflush(stdout);

    // Echo the message back to the client
    write(newsockfd, buffer, strlen(buffer));
    printf("Echoed the message back to the client.\n");
    fflush(stdout);

    // Close the sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}
