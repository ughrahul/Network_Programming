#include <stdio.h>      // For input/output
#include <stdlib.h>     // For exit(), atoi()
#include <unistd.h>     // For fork(), exec()
#include <sys/types.h>  // For pid_t type
#include <sys/wait.h>   // For waitpid() in parent process

int main()
{
    pid_t pid;

    // Create a child process
    pid = fork();

    if (pid < 0)
    {
        // Fork failed
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child Process (PID: %d) executing new program...\n", getpid());

        // Execute a new program using exec()
        execl("/bin/ls", "ls", "-l", NULL);

        // If exec() fails
        perror("Exec failed");
        exit(1);
    }
    else
    {
        // Parent process
        printf("Parent Process (PID: %d) waiting for child (PID: %d)...\n", getpid(), pid);
        wait(NULL); // Wait for child to complete
        printf("Child process finished execution.\n");
    }

    return 0;
}
