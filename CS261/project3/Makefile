# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
CFLAGS  = -std=c99 -pedantic -Wall


all: proj3

run: proj3
	./proj3

proj3: proj3.c binaryTree.c binaryTree.h
	$(CC) $(CFLAGS) -o proj3 proj3.c binaryTree.c

clean:
	$(RM) *.o proj3
