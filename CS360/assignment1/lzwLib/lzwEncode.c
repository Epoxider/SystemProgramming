#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "sequence.h"
#include "bitStream.h"
#include "dict.h"
#include "lzw.h"

bool lzwEncode(unsigned int bits, unsigned int maxBits,
               int (*readFunc)(void* context),
               void (*writeFunc)(unsigned char c, void* context),
               void* context) {

    unsigned int code;
    int hashSize = 1024*1024;
    unsigned int nextCode = 0;
    bool bitIncreasePending = false;
    Dict* dict = newDict(hashSize);
    assert( dict != NULL );

    BitStream* bitStream = openOutputBitStream(writeFunc, context);
    assert( bitStream != NULL );

    for( nextCode = 0; nextCode < 256; nextCode++ ) {
        Sequence *s = newSequence((unsigned char)nextCode, hashSize);
        insertDict(dict, s, nextCode);
    }
   
    // 2: nextCode ← 256  (for loop did this for us)

    int C = (*readFunc)(context);
    assert( C < 256 );
    if( C < 0 ) {
        printf("There was no data to be processed. Finishing\n");
        deleteDictDeep(dict);
        return false;
    }

    // 3: Create new Sequence W containing the first byte of data.
    Sequence *W = newSequence((unsigned char)C, hashSize); 

    C = (*readFunc)(context);
    assert( C < 256 );

    // 4: while there is still data to be read do
    while( C >= 0 ) {
        // 5: C ← Next byte of data from input
        //
        // 6: Create new Sequence X using W appended with C
        Sequence *X = copySequenceAppend(W, (unsigned char)C, hashSize);

        // 7: if X ∈ D then
        if( searchDict(dict, X, &code) == true ) {

            // 8: W ← X
            Sequence * oldW = W;
            W = X;
            deleteSequence(oldW);
            oldW = NULL;

        // 9: else
        } else {

            // 10: find W in D
            assert( searchDict(dict, W, &code) == true );

            // 11: Output code assigned to sequence W
            outputBits(bitStream, bits, code);
            deleteSequence(W);

            // Increasing the bit size where the bitIncreasePending
            // worked for encoding and decoding but didn't match
            // instructor code.  Delay increasing bit size by one
            // cycle using this flag.
            if( bitIncreasePending ) {
                bitIncreasePending = false;
                bits++;
            }

            W = NULL;

            // TODO: Compute if more codes are permitted
            unsigned int maxCodeForBits = (0x01 << (bits)) - 1;

            // 12: if more codes are permitted then
            if( nextCode <= maxCodeForBits ) {
                // 13: Insert X into D, assigning nextCode as its code
                insertDict(dict, X, nextCode);

                // 14: nextCode ← nextCode + 1
                nextCode++;
            }
            else
            {
                deleteSequence(X);
            }

            if( (nextCode > maxCodeForBits) && (bits < maxBits)) {
                printf("We used all codes for %d bits\n", bits);
                // We need to send one more code with the old bits
                // so set flag to indicate this.
                bitIncreasePending = true;
            }

            // 15: Create new sequence W with just character C
            W = newSequence((unsigned char)C, hashSize); 
        }

        C = (*readFunc)(context);
        assert( C < 256 );
    }
    // 16: Find W in D and output its assigned code
    assert( searchDict(dict, W, &code) == true );
    outputBits(bitStream, bits, code);
    deleteSequence(W);
    W = NULL;

    closeAndDeleteBitStream(bitStream);

    deleteDictDeep(dict);
    return true;
}
