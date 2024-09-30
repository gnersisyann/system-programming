#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <sys/stat.h>

int main (int argc, char **argv)
{
	if (argc == 2)
	{
		struct stat fileStat;
		if (stat(argv[1], &fileStat) == -1)
		{
			std::cout<<"Error: file does not exist, code "<<errno<<std::endl;
			return errno;
		}
		int size = fileStat.st_size;
		int fd = open(argv[1], O_WRONLY);
		if (fd == -1)
		{
			std::cout<<"Error: cant open file, code "<<errno<<std::endl;
			return errno;
		}
		if (lseek(fd, 0, SEEK_SET) == -1)
		{
			std::cout<<"Error: unable to seek the start of the file, code "<<errno<<std::endl;
			return errno;
		}
		for (int i = 0; i < size; i++)
		{
			if (write(fd, "\0", 1) == -1)
			{
				std::cout<<"Error: unable to write in the file, code "<<errno<<std::endl;
				close (fd);
				return errno;
			}
		}
		close (fd);
		if (unlink(argv[1]) == -1)
		{
			std::cout<<"Error: unable to delete the file, code "<<errno<<std::endl;
			return errno;
		}
	}
	else
	{
		std::cout<<"Invalid arguments, use [file path]"<<std::endl;
		return 1;
	}
	std::cout<<"File has been successfully deleted"<<std::endl;
	return 0;
}
