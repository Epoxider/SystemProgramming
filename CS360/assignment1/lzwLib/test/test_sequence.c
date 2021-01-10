#include <stdio.h>
#include <stdlib.h>   // for definition of NULL
#include "sequence.h"

void myWriteFunc(unsigned char c, void* context) {
    printf("\tmyWriteFunc: %c\n", c);
}




Sequence* copySequenceAppend(Sequence* sequence,
                             unsigned char addByte, 
                             unsigned int hashSize);

void outputSequence(Sequence* sequence,
                    void (*writeFunc)(unsigned char c, void* context),
                    void* context);

bool identicalSequences(Sequence* a, Sequence* b);
#define HASH_SIZE (1024)

void test1() {
    printf("\n\ntest1\n---------------------------------\n");

    printf("Creating sequence 's' using newSequence('H')\n");
    Sequence* s = newSequence('H', HASH_SIZE);

    printf("Creating sequence 's2'using copySequenceAppend(s, 'e')\n");
    Sequence *s2 = copySequenceAppend(s, 'e', HASH_SIZE);

    printf("Calling outputSequence(s)\n");
    outputSequence(s, myWriteFunc, NULL);

    printf("Calling outputSequence(s2)\n");
    outputSequence(s2, myWriteFunc, NULL);

    printf("Calling deleteSequence(s)\n");
    deleteSequence(s);

    printf("Calling deleteSequence(s2)\n");
    deleteSequence(s2);
}

void test2() {
    printf("\n\ntest2\n---------------------------------\n");

    printf("Creating sequence 'sCurrent' using newSequence('A')\n");
    Sequence* sCurrent = newSequence('A', HASH_SIZE);

    char letter = 'B';
    for( int i = 0; i < 10; i++) {
        printf("Loop %d: Creating sequence 'sNext' using copySequenceAppend(sCurrent, '%c')\n", i, letter);
        Sequence *sNext = copySequenceAppend(sCurrent, letter, HASH_SIZE);

        printf("Loop %d: Calling deleteSequence() on old sequence\n", i);
        deleteSequence(sCurrent);
        sCurrent = NULL;

        // Now, point sCurrent to point to the newly created sequence
        sCurrent = sNext;
        letter++;
    }

    printf("Calling outputSequence(sCurrent)\n");
    outputSequence(sCurrent, myWriteFunc, NULL);

    printf("Calling deleteSequence(sCurrent)\n");
    deleteSequence(sCurrent);
}

int main( int argc, char ** argv) {
    test1();
    test2();
    return 0;
}
