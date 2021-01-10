#include <stdio.h>
#include <stdlib.h>   // for definition of NULL
#include "bitStream.h"

void myWriteFunc(unsigned char c, void* context) {
    char bitsAsString[9];
    bitsAsString[8] = 0;  // Null terminator
    for(int i = 0; i < 8; i++) {
        bitsAsString[i] = (((c >> (7-i)) & 0x01) > 0) ? '1' : '0';
    }

    printf("\tmyWriteFunc: %s\n", bitsAsString);
}

void test1() {
    printf("\n\ntest1\n---------------------------------\n");
    unsigned int code;
    BitStream* bs = openOutputBitStream(myWriteFunc, NULL);

    printf("Writing bits 1\n");
    code = 0x2;  // Binary 00000001
    outputBits(bs, 1, code);

    printf("Writing bits 0\n");
    code = 0x1;  // Binary 00000000
    outputBits(bs, 1, code);

    printf("Writing bits 1 1\n");
    code = 0x0;  // Binary 00000011
    outputBits(bs, 2, code);

    printf("Writing bits 0\n");
    code = 0x0;  // Binary 00000000
    outputBits(bs, 1, code);

    printf("Writing bits 1 1 1\n");
    code = 0x7;  // Binary 00000111
    outputBits(bs, 3, code);

    printf("Calling closeAndDeleteBitStream()\n");

    closeAndDeleteBitStream(bs);
}

void test2() {
    printf("\n\ntest2\n---------------------------------\n");
    unsigned int code;
    BitStream* bs = openOutputBitStream(myWriteFunc, NULL);

    printf("Writing bits 1 1 1\n");
    code = 0xF;  // Binary 00000111
    outputBits(bs, 3, code);

    printf("Writing bits 0 0 0\n");
    code = 0x1;  // Binary 00000000
    outputBits(bs, 3, code);

    printf("Writing bits 1 1 1\n");
    code = 0x7;  // Binary 00000111
    outputBits(bs, 3, code);

    printf("Writing bits 0 0\n");
    code = 0x0;  // Binary 00000000
    outputBits(bs, 2, code);

    printf("Writing bits 1 1\n");
    code = 0x3; // Binary 00000011
    outputBits(bs, 2, code);

    printf("Calling closeAndDeleteBitStream()\n");

    closeAndDeleteBitStream(bs);
}

void test3() {
    printf("\n\ntest3\n---------------------------------\n");
    unsigned int code;
    BitStream* bs = openOutputBitStream(myWriteFunc, NULL);

    for(int i = 0; i < 5; i++) {
        printf("Writing bits 1\n");
        code = 0xFF;  // Binary 11111111
        outputBits(bs, 1, code);  // only write right-most 1 bit

        printf("Writing bits 0 0\n");
        code = 0x0;  // Binary 00000000
        outputBits(bs, 2, code);  // only write righit-most 2 bits
    }

    printf("Calling closeAndDeleteBitStream()\n");

    closeAndDeleteBitStream(bs);
}

void test4() {
    printf("\n\ntest4\n---------------------------------\n");
    unsigned int code;
    BitStream* bs = openOutputBitStream(myWriteFunc, NULL);

    printf("Writing bits 1\n");
    code = 0xFF;  // Binary 11111111
    outputBits(bs, 1, code);  // only write right-most 1 bit

    for(int i = 0; i < 7; i++) {
        printf("Writing no bits\n");
        code = 0xFF;  // Binary 11111111
        outputBits(bs, 0, code);  // only write right-most 1 bit
    }

    printf("Writing bits 1\n");
    code = 0xFF;  // Binary 11111111
    outputBits(bs, 1, code);  // only write right-most 1 bit

    printf("Calling closeAndDeleteBitStream()\n");

    closeAndDeleteBitStream(bs);
}

int main( int argc, char ** argv) {
    test1();
    test2();
    test3();
    test4();
    return 0;
}
