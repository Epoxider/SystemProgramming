#ifndef BITSTREAM_H
#define BITSTREAM_H
#include <stdbool.h>

#define MAX_CODE_BITS   24
#define CHAR_BITS       8

typedef struct _bitStream {
    unsigned int data;
    unsigned int bitsUsed;
    int (*readFunc)(void* context);
    void (*writeFunc)(unsigned char c,void* context);
    void *context;
} BitStream;

BitStream* openInputBitStream(int (*readFunc)(void* context), void* context);

BitStream* openOutputBitStream(void (*writeFunc)(unsigned char c, void* context),void* context);

void closeAndDeleteBitStream(BitStream* bs);

void outputBits(BitStream* bs, unsigned int nBits, unsigned int  code);

bool readInBits(BitStream* bs, unsigned int nBits, unsigned int* code);

#endif
