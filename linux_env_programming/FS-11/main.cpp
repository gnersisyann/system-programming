#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <iostream>

#define BUFSIZE 4096

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        std::cerr << "Invalid arguments, use: <src> <dst> <src_offset> <dst_offset> <bytes>" << std::endl;
        return 1;
    }
    const char *src = argv[1];
    const char *dst = argv[2];
    off_t src_offset = atoi(argv[3]);
    off_t dst_offset = atoi(argv[4]);
    int bytes = atoi(argv[5]);
    int res = bytes;
    char BUF[BUFSIZE];
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0)
    {
        std::cerr << "Error opening source file" << std::endl;
        return errno;
    }
    int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0)
    {
        std::cerr << "Error opening source file" << std::endl;
        close(src_fd);
        return errno;
    }
    if (lseek(src_fd, src_offset, SEEK_SET) < 0)
    {
        std::cerr << "Error moving offset in src" << std::endl;
        close(src_fd);
        close(dst_fd);
        return errno;
    }
    if (lseek(dst_fd, dst_offset, SEEK_SET) < 0)
    {
        std::cerr << "Error moving offset in dst" << std::endl;
        close(src_fd);
        close(dst_fd);
        return errno;
    }
    int bytes_read, bytes_written;
    while ((bytes > 0 && (bytes_read = read(src_fd, BUF, std::min(bytes, BUFSIZE))) > 0))
    {
        bytes_written = write(dst_fd, BUF, bytes_read);
        if (bytes_written < 0)
        {
            std::cerr << "Error writing in dst" << std::endl;
            close(src_fd);
            close(dst_fd);
            return errno;
        }
        bytes -= bytes_written;
    }
    if (bytes_read < 0)
    {
        std::cerr << "Error reading from src" << std::endl;
        close(src_fd);
        close(dst_fd);
        return errno;
    }
    std::cout << "Successfully copied " << res << " bytes from src to dst" << std::endl;
    close(src_fd);
    close(dst_fd);
    return 0;
}
