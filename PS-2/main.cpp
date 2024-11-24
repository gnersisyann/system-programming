#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

void split_input(char *input, char **args)
{
    char *token = strtok(input, " ");
    int index = 0;
    while (token != nullptr)
    {
        args[index++] = token;
        token = strtok(nullptr, " ");
    }
    args[index] = nullptr;
}

bool is_exit_command(char *command)
{
    char *tmp = command;
    while (*tmp == ' ') tmp++;
    return (strncmp(tmp, "exit", 4) == 0);
}

int execute_command(char *command)
{
    if (is_exit_command(command)) return 1;

    char *argv[256];
    char *redirect = strchr(command, '>');
    bool append = false;

    if (redirect != nullptr)
    {
        *redirect = '\0';
        redirect++;
        if (*redirect == '>')
        {
            append = true;
            *redirect = '\0';
            redirect++;
        }
        while (*redirect == ' ' || *redirect == '\t') redirect++;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        std::cout << "Fork failed" << std::endl;
        return -1;
    }
    else if (pid == 0)
    {
        if (redirect)
        {
            int fd;
            if (append)
                fd = open(redirect, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                fd = open(redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                std::cerr << "Error opening/creating file" << std::endl;
                exit(1);
            }
            dup2(fd, 1);
            close(fd);
        }

        split_input(command, argv);
        execvp(argv[0], argv);
        std::cerr << argv[0] << ": command not found" << std::endl;
        exit(1);
    }
    else
    {
        int status;
        wait(&status);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

int process_command(char *command)
{
    char *and_pos = nullptr;
    char *or_pos = nullptr;
    int result;

    while (true)
    {
        and_pos = strstr(command, "&&");
        or_pos = strstr(command, "||");

        if (and_pos != nullptr)
        {
            *and_pos = '\0';
            result = execute_command(command);
            if (result == 1) return 1;
            command = and_pos + 2;
        }
        else if (or_pos != nullptr)
        {
            *or_pos = '\0';
            result = execute_command(command);
            if (result != 0) break;
            command = or_pos + 2;
        }
        else
        {
            result = execute_command(command);
            if (result == 1) return 1;
            break;
        }
    }
    return 0;
}

int do_command(char *input)
{
    char *command = strtok(input, ";");
    while (command != nullptr)
    {
        while (*command == ' ') command++;

        if (is_exit_command(command)) return 1;

        if (process_command(command)) return 1;

        command = strtok(nullptr, ";");
    }
    return 0;
}

int main()
{
    char input[256];
    while (true)
    {
        std::cout << ">: ";
        std::cin.getline(input, 256);
        if (do_command(input) == 1) break;
    }
    return 0;
}
