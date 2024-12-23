#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <ostream>

#include "error_handler.h"

int main()
{
    constexpr size_t ARRAY_SIZE = 1000;
    int shm_fd = shm_open("/shared_array", O_RDWR, 0644);
    if (shm_fd < 0) error_handler("shm_open error");

    void* addr = mmap(nullptr, ARRAY_SIZE * sizeof(bool), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) error_handler("mmap error");
    bool* arr = (bool*)addr;

    int input;
    std::cin >> input;
    while (input < 0 || input > 1000)
    {
        std::cerr << "invalid input (0 <= N <= 1000), try again" << std::endl;
        std::cin >> input;
    }
    if (arr[input])
        std::cout << "Yes" << std::endl;
    else
        std::cout << "No" << std::endl;

    if (munmap(addr, ARRAY_SIZE * sizeof(bool))) error_handler("munmap error");
    if (close(shm_fd) < 0) error_handler("close error");
}