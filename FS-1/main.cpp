#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>

int main (int argc, char **argv)
{
	if (argc > 1)
	{
		int fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			std::cout<<"Error opening file, code:"<<errno<<std::endl;
		const int BUFFER_SIZE = 256;
		char buf[BUFFER_SIZE];
		int bytes_read;
		while((bytes_read = read(fd, buf, BUFFER_SIZE)) > 0)
			std::cout.write(buf, bytes_read);
		if (bytes_read == -1)
			std::cout<<"Error reading file, code:"<<errno<<std::endl;
		if (close(fd) == -1)
			std::cout<<"ERror closing file, code:"<<errno<<std::endl;
	}
	else
		std::cout<<"Invalid arguments"<<std::endl;

}
