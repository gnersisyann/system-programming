#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    int pipefd1[2];
    int pipefd2[2];
    pid_t first_cpid;
    pid_t second_cpid;

    if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0)
    {
        std::cout << "pipe() error" << std::endl;
        exit(errno);
    }
    if ((first_cpid = fork()) < 0)
    {
        std::cerr << "Fork error" << std::endl;
        exit(errno);
    }
    else if (first_cpid == 0)  // first child
    {
        close(pipefd1[0]);
        close(pipefd2[1]);
        std::string pid = std::to_string(getpid());
        if (write(pipefd1[1], pid.c_str(), pid.size()) < 0)
        {
            std::cerr << "Error writing to pipe" << std::endl;
            exit(errno);
        }
        char buf[12];
        int bytes_read = read(pipefd2[0], buf, 12);
        buf[bytes_read] = '\0';
        std::cout << "I am first child process, my pid is " << getpid() << " and i got second child process pid -> "
                  << buf << std::endl;
    }
    else
    {
        if ((second_cpid = fork()) < 0)
        {
            std::cerr << "Fork error" << std::endl;
            exit(errno);
        }
        else if (second_cpid == 0)  // second child
        {
            close(pipefd1[1]);
            close(pipefd2[0]);
            std::string pid = std::to_string(getpid());
            if (write(pipefd2[1], pid.c_str(), pid.size()) < 0)
            {
                std::cerr << "Error writing to pipe" << std::endl;
                exit(errno);
            }
            char buf[12];
            int bytes_read = read(pipefd1[0], buf, 12);
            buf[bytes_read] = '\0';
            std::cout << "I am second child process, my pid is " << getpid() << " and i got first child process pid -> "
                      << buf << std::endl;
        }
        else  // parent
        {
            wait(NULL);
            wait(NULL);
            close(pipefd1[0]);
            close(pipefd1[1]);
            close(pipefd2[0]);
            close(pipefd2[1]);

            return 0;
        }
    }
}