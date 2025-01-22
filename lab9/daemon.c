#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

void daemonize()
{
    pid_t pid;

    // Fork the process
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid > 0)
    {
        // Exit the parent process
        exit(0);
    }

    // Create a new session for the child process
    if (setsid() < 0)
    {
        perror("setsid");
        exit(1);
    }

    // Fork again to ensure the daemon cannot acquire a controlling terminal
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid > 0)
    {
        // Exit the second parent process
        exit(0);
    }

    // Change the working directory to root to avoid blocking file systems
    if (chdir("/") < 0)
    {
        perror("chdir");
        exit(1);
    }

    // Set the file mode creation mask to 0
    umask(0);

    // Redirect standard input, output, and error to /dev/null
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int dev_null = open("/dev/null", O_RDWR);
    if (dev_null < 0)
    {
        perror("open");
        exit(1);
    }
    dup2(dev_null, STDIN_FILENO);
    dup2(dev_null, STDOUT_FILENO);
    dup2(dev_null, STDERR_FILENO);
}

void write_log()
{
    FILE *logfile = fopen("/tmp/daemon.log", "a");
    if (logfile == NULL)
    {
        perror("fopen");
        exit(1);
    }

    while (1)
    {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);

        // Log the current timestamp to the log file
        fprintf(logfile, "%04d-%02d-%02d %02d:%02d:%02d - Daemon is running.\n",
                tm_info->tm_year + 1900,
                tm_info->tm_mon + 1,
                tm_info->tm_mday,
                tm_info->tm_hour,
                tm_info->tm_min,
                tm_info->tm_sec);
        fflush(logfile);

        sleep(5); // Sleep for 5 seconds
    }

    fclose(logfile);
}

int main()
{
    // Turn this process into a daemon
    daemonize();

    // Start logging
    write_log();

    return 0;
}
