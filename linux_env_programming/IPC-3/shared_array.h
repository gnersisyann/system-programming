#pragma once
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>
#include <string>

class shared_array
{
   private:
    std::string name_;
    size_t size_;
    int shm_fd;
    int* array;
    void* addr;
    sem_t* sem;
    // int address;

   public:
    shared_array(const char* name, size_t size) : name_(std::string(name)), size_(size)
    {
        if (size_ > 1000000000)
        {
            throw std::invalid_argument("Size exceeds maximum allowed value");
        }

        shm_fd = shm_open(name_.c_str(), O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1)
        {
            throw std::runtime_error("Failed to create shared memory");
        }

        if (ftruncate(shm_fd, size_ * sizeof(int)) == -1 && errno != EINVAL)
        {
            throw std::runtime_error("Failed to set size of shared memory");
        }

        addr = mmap(NULL, size_ * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (addr == MAP_FAILED)
        {
            throw std::runtime_error("Failed to map shared memory");
        }
        array = static_cast<int*>(addr);

        sem = sem_open("/sem_shared_array", O_CREAT, 0666, 1);
        if (sem == SEM_FAILED)
        {
            throw std::runtime_error("Failed to create semaphore");
        }
    }

    ~shared_array()
    {
        munmap(addr, size_ * sizeof(int));
        close(shm_fd);
        sem_close(sem);
    }

    int& operator[](int index)
    {
        if (index < 0 || static_cast<size_t>(index) >= size_)
        {
            throw std::out_of_range("Index out of range");
        }
        // lock();
        // address = array[index];
        // unlock();
        return array[index];
    }

    void lock() { sem_wait(sem); }
    void unlock() { sem_post(sem); }
};
