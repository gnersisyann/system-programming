#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "error_handler.h"

int main()
{
    constexpr size_t ARRAY_SIZE = 1000;
    int shm_fd = shm_open("/shared_array", O_RDWR, 0644);
    if (shm_fd < 0) error_handler("shm_open error");

    void* addr = mmap(nullptr, ARRAY_SIZE * sizeof(bool), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) error_handler("mmap error");
    bool* arr = (bool*)addr;

    arr[0] = false;
    arr[1] = false;
    for (int i = 2; i * i < ARRAY_SIZE; ++i)
    {
        if (arr[i] == true)
        {
            for (int j = i * i; j < ARRAY_SIZE; j += i)
            {
                if (j % i == 0)
                {
                    arr[j] = false;
                }
            }
        }
    }

    if (munmap(addr, ARRAY_SIZE * sizeof(bool))) error_handler("munmap error");
    if (close(shm_fd) < 0) error_handler("close error");
}