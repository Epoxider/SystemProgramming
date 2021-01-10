#include <stdlib.h>
#include <sequence.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

void computeHash(Sequence * s)
{
    assert(s != NULL);
    unsigned long hash = 5381;

    for(int i = 0; i < s->length; i++ ) { 
        hash = ((hash << 5) + hash) + s->data[i]; /* hash * 33 + c */
    }
    s->hashValue = hash % s->hashSize;
}

Sequence* newSequence(unsigned char firstByte, unsigned int hashSize) {
    Sequence * s = (Sequence *)malloc( sizeof(Sequence) );
    assert( s != NULL );

    s->data = (unsigned char *)malloc( 1 * sizeof(char) );
    s->length = 1;
    assert( s->data != NULL );

    s->data[0] = firstByte;

    s->hashSize = hashSize;
    computeHash( s );

    return s;
}

void deleteSequence(Sequence* sequence) {
    assert( sequence != NULL );
    assert( sequence->data != NULL );

    free( sequence->data );
    free( sequence );
}

Sequence* copySequenceAppend(Sequence* sequence, unsigned char addByte, unsigned int hashSize) {
    Sequence * s = NULL;

    assert( sequence != NULL );
    assert( sequence->data != NULL );

    s = (Sequence *)malloc( sizeof(Sequence) );
    assert( s != NULL );

    s->length = sequence->length + 1;
    s->data = (unsigned char *)malloc( s->length * sizeof(char) );
    assert( s->data != NULL );

    for(int i = 0; i < sequence->length; i++ ) {
        s->data[i] = sequence->data[i];
    }
    s->data[s->length - 1] = addByte;

    s->hashSize = hashSize;
    computeHash( s );

    return s;
}

void outputSequence(Sequence* s,
                    void (*writeFunc)(unsigned char c, void* context), void* context) {
    assert( s != NULL );
    assert( s->data != NULL );

    // Write each character to the output write function
    for( int i = 0; i < s->length; i++ ) {
        (*writeFunc)( s->data[i], context );
    }
}

bool identicalSequences(Sequence* a, Sequence* b) {
    assert( a != NULL );
    assert( b != NULL );

    if( (a == NULL) || (a->data == NULL) || (b == NULL) || (b->data == NULL) ) {
        //printf("\n\nidenticalSequences: We had a NULL ptr\n");
        return false;
    }
    if( (a->length != b->length) || (a->hashSize != b->hashSize) ) {
        //printf("\n\nidenticalSequences: Lengths don't match\n");
        return false;
    }

    for( int i = 0; i < a->length; i++ ) {
        if( a->data[i] != b->data[i] ) {
            //printf("\n\nidenticalSequences: Mismatch at index %d, %c != %c\n", i, a->str[i], b->str[i] );
            return false;
        }
    }

    return true;
}
