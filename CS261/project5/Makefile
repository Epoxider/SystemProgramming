# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
CFLAGS  = -std=c99 -pedantic -Wall


all: proj5

run: proj5
	cat test2.huf | ./proj5

proj5: proj5.c binary_tree.c huffman_decoder.c
	$(CC) $(CFLAGS) -o proj5 proj5.c binary_tree.c huffman_decoder.c

clean:
	$(RM) *.o proj5
