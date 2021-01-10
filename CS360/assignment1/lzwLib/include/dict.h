#ifndef DICT_H
#define DICT_H
#include <stdbool.h>

#include "sequence.h"

typedef struct _linked_list_node {
    Sequence *key;
    unsigned int code;
    struct _linked_list_node *next;
} LinkedListNode;

typedef struct _dict {
    // llArray is a pointer to an array of LinkedListNode pointers
    LinkedListNode **llArray;
    unsigned int size;
    unsigned int hashSize;
    unsigned int collisions;
} Dict;

Dict* newDict(unsigned int hashSize);

void deleteDictDeep(Dict* dict);

bool searchDict(Dict* dict, Sequence* key, unsigned int* code);

void insertDict(Dict* dict, Sequence* key, unsigned int  code);

#endif
