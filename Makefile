CC    = g++

FLAGS = -std=c++11 -g -Wall -pedantic

main: main.cpp vm.o
	 $(CC) $(FLAGS) vm.o main.cpp -o main

vm.o: vm.hpp vm.cpp
	$(CC) $(FLAGS) -c vm.cpp -o vm.o

clean:
	rm -f *.o *.out
	rm main