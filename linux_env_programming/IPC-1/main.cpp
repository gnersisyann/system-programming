#include <pwd.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

#include <csignal>
#include <cstring>
#include <iostream>

void new_handler(int signum, siginfo_t* info, void* extra_info)
{
    ucontext_t* context = (ucontext_t*)extra_info;

    struct passwd* pw = getpwuid(info->si_uid);
    printf("Received a SIGUSR1 signal from process[%d] executed by[%d]([%s]).\n", info->si_pid, info->si_uid,
           pw->pw_name);
    printf("State of the context: RIP = [%lld], RAX = [%lld], RBX = [%lld]. \n", context->uc_mcontext.gregs[REG_RIP],
           context->uc_mcontext.gregs[REG_RAX], context->uc_mcontext.gregs[REG_RBX]);
}

int main()
{
    struct sigaction new_sa;
    memset(&new_sa, 0, sizeof(new_sa));
    new_sa.sa_sigaction = new_handler;
    new_sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &new_sa, nullptr);
    std::cout << "Waiting for SIGUSR1...\n";
    while (true) sleep(10);
}