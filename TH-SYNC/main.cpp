/*In this code mutex and spinlock are fast (sometimes mutex, sometimes spinlock), semaphore is slow because in this task
 * it doesn't make sense to run it with multiple threads at the same time. */
#define BUFSIZE 100

#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <iostream>

pthread_mutex_t mutex;
pthread_spinlock_t spinlock;
sem_t semaphore;

class Integer
{
   private:
    int inner_value = 0;

   public:
    Integer(int value) : inner_value(value) {}
    void inc() { inner_value++; }
    int get_inner_value() { return inner_value; }
};

void write_result(int& fd, char* buffer, double time, int type)
{
    int len;
    switch (type)
    {
        case 1:
            len = snprintf(buffer, BUFSIZE, "Duration with using mutex is %.8f\n", time);
            break;
        case 2:
            len = snprintf(buffer, BUFSIZE, "Duration with using spinlock is %.8f\n", time);
            break;
        case 3:
            len = snprintf(buffer, BUFSIZE, "Duration with using semaphore is %.8f\n", time);
            break;
        default:
            break;
    }
    write(fd, buffer, len);
}

void* increment_mutex(void* arg)
{
    Integer* num = (Integer*)arg;
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 10000; ++i)
    {
        num->inc();
    }
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

void* increment_semaphore(void* arg)
{
    Integer* num = (Integer*)arg;
    sem_wait(&semaphore);
    for (int i = 0; i < 10000; ++i)
    {
        num->inc();
    }
    sem_post(&semaphore);
    return nullptr;
}

void* increment_spinlock(void* arg)
{
    Integer* num = (Integer*)arg;
    pthread_spin_lock(&spinlock);
    for (int i = 0; i < 10000; ++i)
    {
        num->inc();
    }
    pthread_spin_unlock(&spinlock);
    return nullptr;
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);

    Integer* num = new Integer(0);
    int thread_count = 50;
    pthread_t* tids = new pthread_t[thread_count];

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i)
    {
        pthread_create(&tids[i], NULL, increment_mutex, num);
    }

    for (int i = 0; i < thread_count; ++i)
    {
        pthread_join(tids[i], NULL);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mutex_duration = end_time - start_time;
    std::cout << "Duration with using mutex is " << mutex_duration.count() << std::endl;

    pthread_mutex_destroy(&mutex);

    pthread_spin_init(&spinlock, 0);

    delete num;
    num = new Integer(0);

    start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i)
    {
        pthread_create(&tids[i], NULL, increment_spinlock, num);
    }

    for (int i = 0; i < thread_count; ++i)
    {
        pthread_join(tids[i], NULL);
    }
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> spinlock_duration = end_time - start_time;
    std::cout << "Duration with using spinlock is " << spinlock_duration.count() << std::endl;

    pthread_spin_destroy(&spinlock);

    delete num;
    sem_init(&semaphore, 0, 0);
    num = new Integer(0);

    start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i)
    {
        pthread_create(&tids[i], NULL, increment_semaphore, num);
    }

    for (int i = 0; i < thread_count; ++i)
    {
        pthread_join(tids[i], NULL);
    }
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> semaphore_duration = end_time - start_time;
    std::cout << "Duration with using semaphore is " << semaphore_duration.count() << std::endl;
    sem_destroy(&semaphore);

    delete num;
    delete[] tids;
    int fd;
    if ((fd = open("results.txt", O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0)
    {
        std::cerr << "Error opening/creating file" << std::endl;
        exit(errno);
    }
    char buffer[BUFSIZE];
    write_result(fd, buffer, mutex_duration.count(), 1);
    write_result(fd, buffer, spinlock_duration.count(), 2);
    write_result(fd, buffer, semaphore_duration.count(), 3);
    write(fd, "\n", 1);
    close(fd);
    return 0;
}
