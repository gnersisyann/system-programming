#include <csignal>
#include <cstring>
#include <iostream>

int usr2_count = 0;

void usr1_action(int signum, siginfo_t* info, void* extra_info) {}

void usr2_action(int signum, siginfo_t* info, void* extra_info) { ++usr2_count; }

void int_action(int signum, siginfo_t* info, void* extra_info)
{
    std::cout << usr2_count << std::endl;
    exit(0);
}

int main()
{
    struct sigaction new_usr1;
    memset(&new_usr1, 0, sizeof(new_usr1));
    new_usr1.sa_sigaction = usr1_action;
    new_usr1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &new_usr1, nullptr);

    struct sigaction new_int;
    memset(&new_int, 0, sizeof(new_int));
    new_int.sa_sigaction = int_action;
    new_int.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &new_int, nullptr);

    struct sigaction new_usr2;
    memset(&new_usr2, 0, sizeof(new_usr2));
    new_usr2.sa_sigaction = usr2_action;
    new_usr2.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &new_usr2, nullptr);

    while (true)
    {
        std::cout << "I am still alive!!" << std::endl;
        sleep(5);
    }
}