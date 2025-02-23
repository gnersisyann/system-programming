#include <pthread.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

pthread_mutex_t mutex;
pthread_cond_t cond;
bool available = true;

typedef struct
{
    const char* message;
    int thread_id;
} ThreadData;

void* print(void* arg)
{
    ThreadData* data = static_cast<ThreadData*>(arg);
    pthread_mutex_lock(&mutex);
    while (!available)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    available = false;
    std::cout << data->message << " " << data->thread_id << std::endl;
    available = true;
    sleep(1);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    delete[] data;
    return nullptr;
}

int main()
{
    pthread_t threads[3];
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);

    for (int i = 0; i < 3; ++i)
    {
        ThreadData* data = new ThreadData;
        data->message = "Hello from thread";
        data->thread_id = i;
        pthread_create(&threads[i], nullptr, print, static_cast<void*>(data));
    }

    for (int i = 0; i < 3; ++i)
    {
        pthread_join(threads[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}