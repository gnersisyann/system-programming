#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>

#include <cstddef>
#include <functional>

class parallel_scheduler
{
   private:
    size_t capacity_;
    pthread_t* threads;

   public:
    parallel_scheduler(size_t capacity);
    ~parallel_scheduler();
    void run(std::function<void()> task);
};

#endif
