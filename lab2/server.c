#include <stdio.h>      // Standard I/O library
#include <netinet/in.h> // Definitions for internet operations
#include <netdb.h>      // Definitions for network database operations
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SERV_TCP_PORT 5035 // Define the server TCP port number

int main(int argc, char **argv)
{
    int sockfd, newsockfd; // Socket file descriptors and client length
    socklen_t clength;
    struct sockaddr_in serv_addr, cli_addr; // Server and client address structures
    char buffer[4096];                      // Buffer for data transfer

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address structure
    serv_addr.sin_family = AF_INET;            // Address family: Internet
    serv_addr.sin_addr.s_addr = INADDR_ANY;    // Accept connections from any IP address
    serv_addr.sin_port = htons(SERV_TCP_PORT); // Set port number, converting to network byte order

    printf("\nStart");

    // Bind the socket to the address and port
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("\nListening...");

    // Listen for incoming connections (up to 5 in the queue)
    listen(sockfd, 5);

    // Accept a connection
    clength = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clength);
    printf("\nAccepted");

    // Read data from the client
    read(newsockfd, buffer, 4096);
    printf("\nClient message: %s", buffer);

    // Echo the message back to the client
    write(newsockfd, buffer, 4096);
    printf("\n");

    // Close the sockets
    close(sockfd);

    return 0;
}
