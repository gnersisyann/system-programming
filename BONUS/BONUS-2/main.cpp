#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <iostream>
#include <string>

pthread_mutex_t mutex;

void *do_job(void *arg)
{
    char file[256];
    snprintf(file, 256, "out_%d", *(int *)arg + 1);
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        std::cerr << "Error opening/creating file" << std::endl;
        exit(errno);
    }
    int num1, num2;
    std::string operation;
    pthread_mutex_lock(&mutex);
    std::cin >> num1 >> num2 >> operation;
    std::string output;
    if (operation == "s")
    {
        output = std::to_string(num1) + " " + operation + " " + std::to_string(num2) + " = " +
                 std::to_string(num1 + num2) + "\n";
        write(fd, output.c_str(), output.size());
        close(fd);
    }
    else if (operation == "ss")
    {
        output = std::to_string(num1) + " " + operation + " " + std::to_string(num2) + " = " +
                 std::to_string(num1 * num1 + num2 * num2) + "\n";
        write(fd, output.c_str(), output.size());
        close(fd);
    }
    else if (operation == "m")
    {
        output = std::to_string(num1) + " " + operation + " " + std::to_string(num2) + " = " +
                 std::to_string(num1 * num2) + "\n";
        write(fd, output.c_str(), output.size());
        close(fd);
    }
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);
    int N;
    std::cin >> N;
    pthread_t *tids = new pthread_t[N];
    int *args = new int(N);
    for (int i = 0; i < N; ++i)
    {
        args[i] = i;
        pthread_create(&tids[i], nullptr, do_job, &args[i]);
    }
    for (int i = 0; i < N; ++i)
    {
        pthread_join(tids[i], nullptr);
    }
    pthread_mutex_destroy(&mutex);
}