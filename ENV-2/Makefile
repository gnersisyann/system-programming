FLAGS = -Wall -Wextra -Werror
CC = g++

all: complex

complex: main.o complex.o sort.o
	@ $(CC) $(FLAGS) main.o complex.o sort.o -o complex
	@ rm -f main.o complex.o sort.o

main.o: main.cpp
	@ $(CC) $(FLAGS) -c main.cpp -o main.o

complex.o: utils/complex.cpp
	@ $(CC) $(FLAGS) -c utils/complex.cpp -o complex.o

sort.o: utils/sort.cpp
	@ $(CC) $(FLAGS) -c utils/sort.cpp -o sort.o

clean:
	@ rm -rf complex
