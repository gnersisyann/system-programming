all: complex

complex: main.o complex.o sort.o
	@ g++ main.o complex.o sort.o -o complex
	@ rm -f main.o complex.o sort.o

main.o: main.cpp
	@ g++ -c main.cpp -o main.o

complex.o: utils/complex.cpp
	@ g++ -c utils/complex.cpp -o complex.o

sort.o: utils/sort.cpp
	@ g++ -c utils/sort.cpp -o sort.o

clean:
	@ rm -rf complex
