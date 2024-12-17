#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Required for inet_addr

#define PORT 5035

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);

    printf("Ready for sending...\n");

    // Connect the socket
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Get the message from the user
    printf("Enter the message to send\n");
    printf("\nClient: ");

    // Read input from the user
    fgets(buffer, sizeof(buffer), stdin);

    // Send the message to the server
    write(sockfd, buffer, sizeof(buffer));

    // Display the echo from the server
    printf("Server Echo: %s", buffer);
    printf("\n");

    // Close the socket after communication is done
    close(sockfd);

    return 0;
}
