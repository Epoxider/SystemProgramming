SYMFORMAT=dwarf
FORMAT=elf

main: stringgt.o main.o
	gcc -m32 -g -o main stringgt.o main.o

main.o: main.c
	gcc -g -c main.c -m32

stringgt.o: stringgt.asm
	nasm -f $(FORMAT) -g -F $(SYMFORMAT) stringgt.asm

clean:
	rm main stringgt.o main.o 

