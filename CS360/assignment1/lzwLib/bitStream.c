#include <stdlib.h>
#include <bitStream.h>
#include <stdio.h>
#include <assert.h>

BitStream* openInputBitStream(int (*readFunc)(void* context), void* context) {
    BitStream * bitStream = (BitStream *)malloc( sizeof(BitStream) );
    bitStream->data = 0;
    bitStream->bitsUsed = 0;
    bitStream->readFunc = readFunc;
    bitStream->writeFunc = NULL;
    bitStream->context = context;
    return bitStream;
}

BitStream* openOutputBitStream(void (*writeFunc)(unsigned char c,void* context),void* context) {
    BitStream * bitStream = (BitStream *)malloc( sizeof(BitStream) );
    bitStream->data = 0;
    bitStream->bitsUsed = 0;
    bitStream->readFunc = NULL;
    bitStream->writeFunc = writeFunc;
    bitStream->context = context;
    return bitStream;
}

void closeAndDeleteBitStream(BitStream* bs) {
    if( bs->writeFunc != NULL ) {
        if( bs->bitsUsed > 0 ) {
            bs->data <<= 8 - bs->bitsUsed;
            (*(bs->writeFunc))( bs->data, bs->context );
        }
    } else {
        assert( bs->bitsUsed == 0 );
    }
    free( bs );
}

void outputBits(BitStream* bs, unsigned int nBits, unsigned int code) {
    assert( bs->writeFunc != NULL );

    for(int i = 0; i < nBits; i++ ) {
        unsigned char bit = (unsigned char)( (code >> ((nBits-1) - i)) & (unsigned int)0x01 );
        bs->data = (bs->data << 1) | bit;
        bs->bitsUsed++;
        if( bs->bitsUsed == 8 ) {
            (*(bs->writeFunc))( (unsigned char)(bs->data), bs->context );
            bs->data = 0;
            bs->bitsUsed = 0;
        }
    }
}

bool readInBits(BitStream* bs, unsigned int nBits, unsigned int* code) {
    assert( bs->readFunc != NULL );
    assert( code != NULL );

    if( bs->bitsUsed <= 0 ) {
        // Need a temp variable that can handle negative values.
        int dataRead = (*(bs->readFunc))(bs->context);
        if( dataRead < 0 ) {
            printf("\nNo wasted bits in input file\n");
            return false;
        }
        bs->data = (unsigned int)dataRead;
        bs->bitsUsed = 8;
    }

    unsigned int tmpCode = 0;

    for(int i = 0; i < nBits; i++ ) {
        if( bs->bitsUsed <= 0 ) {
            // Need a temp variable that can handle negative values.
            int dataRead = (*(bs->readFunc))(bs->context);
            if( dataRead < 0 ) {
                printf("\nThere were %d wasted bits in input file\n", i);
                return false;
            }
            assert( dataRead < 256 );
            bs->data = (unsigned int)dataRead;
            bs->bitsUsed = 8;
        }
        unsigned char bit = (bs->data >> (bs->bitsUsed-1)) & 0x01;
        tmpCode = (tmpCode << 1) | (unsigned int)bit;
        bs->bitsUsed--;
    }
    *code = tmpCode;
    return true;
}
