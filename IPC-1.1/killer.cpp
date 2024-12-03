#include <unistd.h>

#include <csignal>
#include <cstdlib>
int main(int argc, char** argv)
{
    if (argc != 3) return 1;
    pid_t pid = atoi(argv[2]);
    int sig = atoi(argv[1]);
    kill(pid, sig);
}