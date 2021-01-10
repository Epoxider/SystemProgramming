#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "sequence.h"
#include "bitStream.h"
#include "lzw.h"

#define HASH_SIZE (1024)

bool lzwDecode(unsigned int bits, unsigned int maxBits,
               int  (*readFunc )(void* context),
               void (*writeFunc)(unsigned char c, void* context),
               void* context) {

    int maxNumCodes = 0x01 << maxBits;
    int TSize = 0;
    unsigned int currentCode = -1;
    unsigned int previousCode = -1;
    int nBits = bits; // starts out with lowest bits value
    unsigned char C = 0;
    Sequence * W = NULL;

    BitStream* bs = openInputBitStream(readFunc, context);
    assert( bs != NULL );

    // 1: Table T is a table of Sequences, indexed by an integer code. The table
    //        must be large enough to hold as many codes as are permitted by the
    //        maximum code bit width. Initialize table T with entries 0 through 255
    //        with each holding a single character Sequence, one entry for each
    //        character 0 through 255, respectively.
    Sequence ** T = (Sequence **)malloc( maxNumCodes * sizeof(Sequence *));

    for(TSize = 0; TSize < 256; TSize++ ) {
        T[TSize] = newSequence( (unsigned char)TSize, HASH_SIZE);
        assert( T[TSize] != NULL );
    }
    // TSize now has the size of valid codes in table.

    for(int i = 256; i < maxNumCodes; i++ ) {
        T[i] = (Sequence *)NULL;
    }

    // 2: previousCode ← first code read from input
    if( !readInBits(bs, nBits, &previousCode) ) {
        printf("No data to read for decode\n");
        return false;
    }

    // 3: while there are more codes to be read do
    while( readInBits(bs, nBits, &currentCode ) ) {
        // 4: currentCode ← next code from input

        // 5: if currentCode ∈ T then
        if( T[currentCode] != NULL ) {
            // 6: C ← first character of T[currentCode]
            C = T[currentCode]->data[0];
        // 7: else
        } else {
            // 8: C ← first character of T[previousCode]
            C = T[previousCode]->data[0];
        }

        // 9: if T is not full then
        if( TSize < maxNumCodes ) {
            // 10: W ← new sequence using T[previousCode] appended with C
            W = copySequenceAppend( T[previousCode], C, HASH_SIZE);
            assert( W != NULL );

            // 11: add W at next index in T
            T[TSize++] = W;
            if( (TSize >= (0x01<<nBits)) && (nBits < maxBits) ) {
                printf("Bumping bits from %d to %d\n", nBits, nBits+1);
                nBits++;
            }
        }

        // 12: output Sequence T[currentCode]
        outputSequence( T[previousCode], writeFunc, context);
        
        // 13: previousCode ← currentCode
        previousCode = currentCode;
    }

    // Write the final code
    outputSequence( T[previousCode], writeFunc, context);

    for(int i = 0; i < maxNumCodes; i++ ) {
        if( T[i] != NULL ) {
            deleteSequence( T[i] );
            T[i] = NULL;
        }
    }
    free( T );
    T = NULL;

    closeAndDeleteBitStream(bs);
    return true;
}
