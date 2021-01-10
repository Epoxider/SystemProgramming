#include <stdio.h>
#include <stdlib.h>   // for definition of NULL
#include "dict.h"
#include "sequence.h"

Dict* newDict(unsigned int hashSize);



#define HASH_SIZE (1024)

void test1() {
    printf("\n\ntest1\n---------------------------------\n");

    printf("Creating sequence 's' using newSequence('Z')\n");
    Sequence* s = newSequence('Z', HASH_SIZE);
    Sequence* s2 = newSequence('A', HASH_SIZE);

    Dict* d = newDict(HASH_SIZE);

    insertDict(d, s, 123);
    unsigned int code;

    if( searchDict(d, s, &code) ) {
        printf("Found excpedted dictionary: code=%d : GOOD\n", code);
    }
    else
    {
        printf("Did not find excpedted dictionary: BAD\n");
    }

    if( !searchDict(d, s2, &code) ) {
        printf("Did not find dictionary that shouldn't be there: GOOD\n");
    }
    else
    {
        printf("Found dictionary that shouldn't be there: BAD\n");
    }

    deleteDictDeep(d);

    deleteSequence(s2);
}

int main( int argc, char ** argv) {
    test1();
    return 0;
}
