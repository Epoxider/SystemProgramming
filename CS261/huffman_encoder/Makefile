# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
CFLAGS  = -std=c99 -pedantic -Wall


all: main

run: main
	./main words.txt

main: main.c binary_tree.c min_heap.c
	$(CC) $(CFLAGS) -o main main.c binary_tree.c min_heap.c

clean:
	$(RM) *.o main
