
INCLUDE=include/
CFLAGS=-c -std=c99 -Wall -pedantic -I$(INCLUDE) -g 
TESTCFLAGS=-std=c99 -Wall -pedantic -I$(INCLUDE) -g 

liblzw.a: lzwEncode.o lzwDecode.o sequence.o dict.o bitStream.o
	ar cru liblzw.a lzwEncode.o lzwDecode.o sequence.o dict.o bitStream.o
	ranlib liblzw.a

lzwEncode.o: lzwEncode.c $(INCLUDE)lzw.h $(INCLUDE)bitStream.h $(INCLUDE)dict.h $(INCLUDE)sequence.h
	gcc $(CFLAGS) lzwEncode.c

lzwDecode.o: lzwDecode.c $(INCLUDE)lzw.h $(INCLUDE)bitStream.h $(INCLUDE)dict.h $(INCLUDE)sequence.h
	gcc $(CFLAGS) lzwDecode.c

bitStream.o: bitStream.c $(INCLUDE)bitStream.h
	gcc $(CFLAGS) bitStream.c

dict.o: dict.c $(INCLUDE)dict.h $(INCLUDE)sequence.h
	gcc $(CFLAGS) dict.c

sequence.o: sequence.c $(INCLUDE)sequence.h
	gcc $(CFLAGS) sequence.c

clean:
	rm -f liblzw.a lzwEncode.o lzwDecode.o bitStream.o dict.o sequence.o
