#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define CLIENT_TCP_PORT 5035 // Define the server port for communication

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[4098];

    // Loop to continuously send messages until "Quit" is entered
    while (1)
    {
        // Create a socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        // Define server properties
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server's IP address (loopback)
        serv_addr.sin_port = htons(CLIENT_TCP_PORT);        // Server's port number

        printf("[CLIENT]: Ready to connect to server...\n");

        // Connect to the server
        connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        printf("[CLIENT]: Connected to server successfully.\n");

        // Read input from user
        printf("[CLIENT]: Enter message to send to server: ");
        fgets(buffer, 4098, stdin);

        // Check if the user typed "Quit"
        if (strcmp(buffer, "Quit\n") == 0)
        {
            break; // Exit the loop
        }

        // Send message to the server
        write(sockfd, buffer, strlen(buffer));
        printf("[CLIENT]: Message sent to server: %s", buffer);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
