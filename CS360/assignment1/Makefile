
INCLUDE=lzwLib/include/
CFLAGS= -std=c99 -Wall -pedantic -I$(INCLUDE) -g

all: testcomp testdecomp

testcomp: testcomp.c ./lzwLib/liblzw.a
	gcc $(CFLAGS) testcomp.c ./lzwLib/liblzw.a -o testcomp

testdecomp: testdecomp.c ./lzwLib/liblzw.a
	gcc $(CFLAGS) testdecomp.c ./lzwLib/liblzw.a -o testdecomp

clean:
	rm -f testcomp.o testcomp testdecomp.o testdecomp
