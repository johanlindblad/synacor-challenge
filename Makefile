CC    = g++

FLAGS = -std=c++11 -g -Wall -pedantic

main: main.cpp
	 $(CC) $(FLAGS) main.cpp -o main

clean:
	rm -f *.o *.out