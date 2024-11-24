#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <string>

void initialize(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Invalid arguments" << std::endl;
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        std::cerr << "Error opening file" << std::endl;
        exit(2);
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        std::cerr << "Error redirecting input" << std::endl;
        close(fd);
        exit(3);
    }
    close(fd);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }
    initialize(argc, argv);

    std::string input;
    std::cin >> input;

    std::string reversed(input.rbegin(), input.rend());

    std::cout << reversed << std::endl;
    return 0;
}
