#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERV_TCP_PORT 5035 // Locally hosted TCP port

int main(int argc, char **argv)
{
    int sockfd, newsockfd;
    socklen_t clength;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[4098];
    pid_t childpid;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);
    printf("[SERVER]: Server started successfully on port %d.\n", SERV_TCP_PORT);

    // Bind the socket to the address and port
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Main loop to accept and handle incoming connections
    for (;;)
    {
        // Listen for incoming connections with a backlog of 5
        listen(sockfd, 5);
        printf("[SERVER]: Listening for incoming connections...\n");

        // Accept the incoming connection
        clength = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clength);
        printf("[SERVER]: Connection accepted from client\n");

        // Create a new child process for each incoming client connection
        if ((childpid = fork()) == 0)
        {
            // In the child process
            printf("[SERVER]: Child process created successfully. PID: %d\n", getpid());

            // Read client message into buffer.
            read(newsockfd, buffer, 4096);
            printf("[SERVER]: Message received from client: %s\n", buffer);

            // Close the connection with the client
            close(newsockfd);
            return 0; // Exit child process
        }

        // Close the new socket descriptor in the parent process
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
