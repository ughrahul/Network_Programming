#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void get_current_date_time(char *buffer, size_t buffer_size)
{
    time_t now = time(NULL);   //get the current time
    struct tm *local_time = localtime(&now);   //convert to local time
    strftime(buffer, buffer_size, "Current Date and Time: %Y-%m-%d %H:%M:%S", local_time); //format the time
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addr_len = sizeof(address);
    char date_time[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    //runs server continously to accept connections from client
    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addr_len);
        get_current_date_time(date_time, BUFFER_SIZE);
        send(new_socket, date_time, strlen(date_time), 0);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
