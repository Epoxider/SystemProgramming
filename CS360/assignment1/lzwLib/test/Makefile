
INCLUDE=../include/
CFLAGS=-std=c99 -Wall -pedantic -I$(INCLUDE) -g

all: test_bs test_sequence test_hash test_dict test_bits

test_bs: ../bitStream.c test_bs.c
	gcc $(CFLAGS) ../bitStream.c test_bs.c -o test_bs

test_sequence: ../sequence.c test_sequence.c
	gcc $(CFLAGS) ../sequence.c test_sequence.c -o test_sequence

test_dict: ../sequence.c ../dict.c test_dict.c
	gcc $(CFLAGS) ../sequence.c ../dict.c test_dict.c -o test_dict

test_hash: test_hash.c
	gcc $(CFLAGS) test_hash.c -o test_hash

test_bits: test_bits.c
	gcc $(CFLAGS) test_bits.c -o test_bits

clean:
	rm -f test_bs bitStream.o test_sequence sequence.o test_hash  test_dict test_bits test_bits.o

