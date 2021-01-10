#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <stdbool.h>

typedef struct _sequence {
    unsigned int hashValue;
    unsigned int hashSize;
    unsigned char * data;
    unsigned int length;
} Sequence;

Sequence* newSequence(unsigned char firstByte, unsigned int hashSize);

void deleteSequence(Sequence* sequence);

Sequence* copySequenceAppend(Sequence* sequence,
                             unsigned char addByte, 
                             unsigned int hashSize);

void outputSequence(Sequence* sequence,
                    void (*writeFunc)(unsigned char c, void* context),
                    void* context);

bool identicalSequences(Sequence* a, Sequence* b);

#endif
