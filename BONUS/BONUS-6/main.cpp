#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int split_input(char *input, char **args)
{
    char *pipe_pos = strchr(input, '|');
    int pipe_index = -1;

    if (pipe_pos != nullptr)
    {
        pipe_index = pipe_pos - input;
        *pipe_pos = '\0';
    }

    char *token = strtok(input, " ");
    int index = 0;
    while (token != nullptr)
    {
        args[index++] = token;
        token = strtok(nullptr, " ");
    }
    args[index] = nullptr;

    return pipe_index;
}

void do_job(char *input)
{
    char *args1[64];
    char *args2[64];
    int pipe_index = split_input(input, args1);

    if (pipe_index != -1)
    {
        char *second_command = input + pipe_index + 1;
        split_input(second_command, args2);

        int pipefd[2];
        if (pipe(pipefd) < 0)
        {
            std::cerr << "pipe() error" << std::endl;
            exit(1);
        }

        pid_t pid1 = fork();
        if (pid1 < 0)
        {
            std::cerr << "fork() error" << std::endl;
            exit(1);
        }
        else if (pid1 == 0)
        {
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            execvp(args1[0], args1);
            std::cerr << "execvp() error" << std::endl;
            exit(1);
        }

        pid_t pid2 = fork();
        if (pid2 < 0)
        {
            std::cerr << "fork() error" << std::endl;
            exit(1);
        }
        else if (pid2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            execvp(args2[0], args2);
            std::cerr << "execvp() error" << std::endl;
            exit(1);
        }

        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, nullptr, 0);
        waitpid(pid2, nullptr, 0);
    }
    else
    {
        execvp(args1[0], args1);
        std::cerr << "execvp() error" << std::endl;
        exit(1);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./program \"command\" " << std::endl;
        return 1;
    }

    do_job(argv[1]);
    return 0;
}
