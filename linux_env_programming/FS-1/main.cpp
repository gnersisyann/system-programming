#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1)
        {
            std::cout << "Error opening file, code:" << errno << std::endl;
            return errno;
        }
        const int BUFFER_SIZE = 256;
        char buf[BUFFER_SIZE];
        int bytes_read;
        while ((bytes_read = read(fd, buf, BUFFER_SIZE)) > 0) std::cout.write(buf, bytes_read);
        if (bytes_read == -1)
        {
            std::cout << "Error reading file, code:" << errno << std::endl;
            return errno;
        }
        if (close(fd) == -1)
        {
            std::cout << "Error closing file, code:" << errno << std::endl;
            return errno;
        }
    }
    else
    {
        std::cout << "Invalid arguments" << std::endl;
        return 1;
    }
    return 0;
}
