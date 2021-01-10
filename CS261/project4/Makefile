# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
CFLAGS  = -std=c99 -pedantic -Wall


all: proj4

run: proj4
	./proj4 foobar.txt 3

proj3: proj4.c
	$(CC) $(CFLAGS) -o proj4 proj4.c

clean:
	$(RM) *.o proj4
