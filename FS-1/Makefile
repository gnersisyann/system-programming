CC = g++
FLAGS = -Wextra -Wall -Werror
NAME = read-file

all: make

make: main.o
	@ $(CC) $(FLAGS) main.o -o $(NAME)
	@ rm -f main.o

main.o: main.cpp
	@ $(CC) $(FLAGS) -c main.cpp

clean:
	@ rm -rf $(NAME)
