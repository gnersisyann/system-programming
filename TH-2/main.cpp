#include <pthread.h>
#include <unistd.h>

#include <cstddef>
#include <functional>
#include <iostream>
#include <queue>

std::queue<std::function<void()>> tasks;
pthread_cond_t cond;
pthread_mutex_t mutex;
bool stopped = false;

void* working(void* arg)
{
    while (true)
    {
        pthread_mutex_lock(&mutex);
        while (tasks.empty() && !stopped) pthread_cond_wait(&cond, &mutex);
        if (tasks.empty() && stopped)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        auto job = tasks.front();
        tasks.pop();
        pthread_mutex_unlock(&mutex);
        job();
    }
    return nullptr;
}

class parallel_scheduler
{
   private:
    size_t capacity_;
    pthread_t* threads;

   public:
    parallel_scheduler(size_t capacity) : capacity_(capacity)
    {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
        threads = new pthread_t[capacity_];
        for (size_t i = 0; i < capacity_; ++i) pthread_create(&threads[i], nullptr, working, nullptr);
    }

    ~parallel_scheduler()
    {
        pthread_mutex_lock(&mutex);
        stopped = true;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        for (size_t i = 0; i < capacity_; ++i) pthread_join(threads[i], nullptr);
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
        delete[] threads;
    }

    void run(std::function<void()> task)
    {
        pthread_mutex_lock(&mutex);
        tasks.push(task);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
};

int main()
{
    parallel_scheduler scheduler(3);

    scheduler.run([]() { std::cout << "Another task!" << std::endl; });

    for (int i = 0; i < 15; ++i)
    {
        scheduler.run(
            []()
            {
                sleep(1);
                std::cout << "Task in loop!" << std::endl;
            });
    }

    return 0;
}