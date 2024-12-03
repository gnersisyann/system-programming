/*
When a child process kills a parent process it becomes an orphan process and its new parent becomes init.
(it depends on the OS)
If the child process does not have access to kill the parent process,
it returns an error and the parent process is terminated on completion
*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork error\n");
        exit(errno);
    }
    else if (pid == 0)
    {
        printf("Child PID = %d, parent PID = %d\n", getpid(), getppid());

        if (kill(getppid(), SIGTERM) == 0)
        {
            printf("Child process has sended SIGTERM signal to parent process.\n");
        }
        else
        {
            perror("Child process can`t send signal\n");
        }

        sleep(2);
        printf("Child process is ending.\n");
    }
    else
    {
        printf("Parent PID = %d, child PID = %d\n", getpid(), pid);

        sleep(5);
        printf("Parent process is ended\n");
    }

    return 0;
}
