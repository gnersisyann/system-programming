#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>

void print_file_content(const char *file_path)
{
    int fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        std::cerr << "Error opening file " << file_path << std::endl;
        exit(errno);
    };
    char buffer[1024];
    int bytes_read, bytes_written;
    while ((bytes_read = read(fd, buffer, 1024)) > 0)
    {
        bytes_written = write(1, buffer, bytes_read);
        if (bytes_written < 0 || bytes_written != bytes_read)
        {
            std::cerr << "Error writing file" << std::endl;
            close(fd);
            exit(errno);
        }
    }
    if (bytes_read < 0)
    {
        std::cerr << "Error reading file" << std::endl;
        close(fd);
        exit(errno);
    }
    close(fd);
    write(1, "\n", 1);
}

void print_directory_files(const char *dir_name)
{
    DIR *dir = opendir(dir_name);
    if (dir == nullptr)
    {
        std::cerr << "Error opening directory" << std::endl;
        exit(errno);
    }
    struct dirent *dir_read;
    while ((dir_read = readdir(dir)) != nullptr)
    {
        if (strcmp(dir_read->d_name, ".") == 0 || strcmp(dir_read->d_name, "..") == 0)
        {
            continue;
        }
        char filepath[1024];
        snprintf(filepath, 1024, "%s/%s", dir_name, dir_read->d_name);

        struct stat info;
        stat(filepath, &info);

        if (S_ISDIR(info.st_mode))
        {
            print_directory_files(filepath);
        }
        else if (S_ISREG(info.st_mode))
        {
            print_file_content(filepath);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }
    print_directory_files(argv[1]);
    return 0;
}
