#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <iostream>

void do_command(char **argv)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cout << "Fork failed" << std::endl;
        return;
    }
    else if (pid == 0)
    {
        execvp(argv[0], argv);
        std::cerr << "Execution failed" << std::endl;
        exit(1);
    }
    else
    {
        auto start_time = std::chrono::high_resolution_clock::now();

        int status;
        waitpid(pid, &status, 0);

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;

        if (WIFEXITED(status))
        {
            std::cout << "Exit status: " << WEXITSTATUS(status) << std::endl;
        }
        else if (WIFSIGNALED(status))
        {
            std::cout << "Command terminated by signal: " << WTERMSIG(status) << std::endl;
        }

        std::cout << "Duration: " << duration.count() << " seconds" << std::endl;
    }
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }
    do_command(argv + 1);
    return 0;
}
