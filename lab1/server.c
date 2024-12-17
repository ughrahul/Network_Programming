#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // For close()

#define SERV_TCP_PORT 5035 // Define server port

int main(int argc, char **argv)
{
    int sockfd, newsockfd, clength;
    struct sockaddr_in serv_addr, cli_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    printf("\nSERVER HAS BEEN STARTED\n");

    // Bind socket to address
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("\nListening TO THE CLIENT\n");

    // Listen for connections
    listen(sockfd, 1);

    clength = sizeof(cli_addr);

    // Accept a connection
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clength);

    printf("\nAccepted\n");

    // Close sockets
    close(sockfd);
    close(newsockfd);

    return 0;
}
