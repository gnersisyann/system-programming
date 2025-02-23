#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "error_handler.h"

int main()
{
    constexpr size_t ARRAY_SIZE = 1000;
    int shm_fd = shm_open("/shared_array", O_RDWR | O_CREAT, 0644);
    if (shm_fd < 0) error_handler("shm_open error");

    if (ftruncate(shm_fd, ARRAY_SIZE * sizeof(bool)) < 0) error_handler("ftruncate error");
    void* addr = mmap(nullptr, ARRAY_SIZE * sizeof(bool), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) error_handler("mmap error");

    bool* arr = (bool*)addr;
    memset(arr, 1, ARRAY_SIZE * sizeof(bool));
    
    if (munmap(addr, ARRAY_SIZE * sizeof(bool))) error_handler("munmap error");
    if (close(shm_fd) < 0) error_handler("close error");
}