#include <stdio.h>
#include <unistd.h> // For fork()

int main()
{
    pid_t pid = fork(); // Create a child process

    if (pid < 0)
    {
        // Error in fork
        printf("Fork failed\n");
    }
    else if (pid == 0)
    {
        // This block is executed by the child process
        printf("This is the child process.\n");
    }
    else
    {
        // This block is executed by the parent process
        printf("This is the parent process. Child PID is %d\n", pid);
    }

    return 0;
}
