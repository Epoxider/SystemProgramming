SYMFORMAT=dwarf
FORMAT=elf

hw8: hw8.o
	gcc -m32 -nostartfiles -g -o hw8 hw8.o

hw8.o: hw8.asm
	nasm -f $(FORMAT) -g -F $(SYMFORMAT) hw8.asm


