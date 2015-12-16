CC    = g++

FLAGS = -std=c++11 -g -Wall -pedantic

all: main debug

main: main.cpp vm.o
	$(CC) $(FLAGS) vm.o main.cpp -o main

debug: debugger.o vm.o
	$(CC) $(FLAGS) -l readline vm.o debugger.o debug.cpp -o debug

vm.o: vm.hpp vm.cpp
	$(CC) $(FLAGS) -c vm.cpp -o vm.o

debugger.o: debugger.hpp debugger.cpp
	$(CC) $(FLAGS) -c debugger.cpp -o debugger.o

clean:
	rm -f *.o *.out
	rm main
	rm debug