#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <algorithm>
#define buf_size 4096

int handle_error(const char *message)
{
    std::cerr << message << std::endl;
    exit(errno);
}

void handle_data(int size, int src, int dest, char BUF[buf_size], int& data_bytes, int& copied_bytes)
{
	ssize_t bytes_read, bytes_written;
	while (size > 0)
	{
		bytes_read = read(src, BUF, std::min(size, buf_size));
		if (bytes_read < 0)
			handle_error("Error reading from source file");
		if (bytes_read == 0)
			break;
		bytes_written = write(dest, BUF, bytes_read);
		if (bytes_written < 0)
			handle_error("Error writing to destination file");
		data_bytes += bytes_read;
		copied_bytes += bytes_written;
		size -= bytes_read;
	}
}


void handle_hole(int size, int dest, int& hole_bytes, int& copied_bytes)
{
	while (size > 0)
	{	
		int step = std::min(size, buf_size);
		lseek(dest, step, SEEK_CUR);
		hole_bytes += step;
		copied_bytes += step;
		size -= buf_size;
	}
}

int main(int argc, char **argv)
{
    if (argc != 3)
        handle_error("Invalid arguments. Usage: ./copy source-file destination-file");

    int fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1)
        handle_error("Error opening source file");

    int fd_dst = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd_dst == -1)
        handle_error("Error opening destination file");

    char BUF[buf_size];
    off_t pos = 0;
    off_t data_pos, hole_pos;
    int copied_bytes = 0, data_bytes = 0, hole_bytes = 0;
    int size_to_read, size_to_skip;
    bool first_cycle = true;

    while (true)
    {
	    data_pos = lseek(fd_src, pos, SEEK_DATA);
	    hole_pos = lseek(fd_src, pos, SEEK_HOLE);
	    if (first_cycle == true && data_pos >= 0 && hole_pos == lseek(fd_src, pos, SEEK_END))
	    {
		lseek(fd_src, pos, SEEK_SET);
		handle_data(hole_pos - data_pos, fd_src, fd_dst, BUF, data_bytes, copied_bytes);
	    	break;
	    }
	    if (data_pos < 0 && hole_pos < 0)
	    {
		if (errno == ENXIO)
			break;
		else
			handle_error("lseek error");
	    }
	    
	    if (data_pos == hole_pos)
		    break;

	    if (data_pos < hole_pos || hole_pos == -1)
	    {
		    size_to_read = hole_pos - data_pos;
		    if (size_to_read > buf_size) size_to_read = buf_size;
		    handle_data(size_to_read, fd_src, fd_dst, BUF, data_bytes, copied_bytes);
		    pos = hole_pos;
	    }
	    else
	    {
		    size_to_skip = data_pos - pos;
		    handle_hole(size_to_skip, fd_dst, hole_bytes, copied_bytes);
		    pos = data_pos;
	    }
	    first_cycle = false;
    }
    close(fd_src);
    close(fd_dst);
    std::cout << "Successfully copied " << copied_bytes << " bytes (data: " << data_bytes << ", hole: " << hole_bytes << ")." << std::endl;
    return 0;
}
