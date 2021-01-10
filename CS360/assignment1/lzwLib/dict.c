#include <stdlib.h>
#include <dict.h>
#include <stdio.h>
#include <assert.h>

#define NULL ((void *)0)

//create a nwe dictioanry to use
Dict* newDict(unsigned int hashSize) {
    Dict *myDict = (Dict *)malloc( sizeof(Dict) );

    assert( myDict != NULL );

    //store hashSize
    myDict->hashSize = hashSize;

    //allocate memory for the array of Linked List pointers
    myDict->llArray = (LinkedListNode **)malloc( hashSize * sizeof(LinkedListNode *) );

    //if second allocation failed, free up the dictionary and return NULL
    assert( myDict->llArray != NULL );

    // Initialize every entry in the hash table to NULL, indicating empty
    for(int i = 0; i < hashSize; i++ ) {
        myDict->llArray[i] = NULL;
    }
    myDict->collisions = 0;
    myDict->size = 0;

    return myDict;
}

void deleteDictDeep(Dict* dict) {
    for(int i = 0; i < dict->hashSize; i++ ) {

        //create a pointer to walk through the ll
        LinkedListNode * arrayEntry = dict->llArray[i];
        if( arrayEntry != NULL ) {

            // Walk the linked list, delete all the nodes
            while ( arrayEntry != NULL ) {
                LinkedListNode * tmpNode = arrayEntry;
                arrayEntry = arrayEntry->next;
                deleteSequence( tmpNode->key );
                free( tmpNode );
            }
        }
    }

    free( dict->llArray );
    free( dict );
}

bool searchDict(Dict* dict, Sequence* key, unsigned int* code) {
    LinkedListNode * node = NULL;

    // Make sure hashSize's match.  If not, things won't go well
    assert( dict->hashSize == key->hashSize );

    // If this key is in the dictionary, find which linked list it would be in
    node = dict->llArray[key->hashValue];

    // Now, walk the linked list seeing if it exists
    while( node != NULL ) {
        if( identicalSequences(key, node->key) ) {
            *code = node->code;
            return true;
        }
        node = node->next;
    }

    return false;
}

void insertDict(Dict* dict, Sequence* key, unsigned int code) {

    assert( dict != NULL );
    assert( key != NULL );
    assert( key->data != NULL );

    LinkedListNode * node = dict->llArray[key->hashValue];
    LinkedListNode * prevNode = NULL;

    assert( dict->hashSize == key->hashSize );

    if(node == NULL) {
        node = (LinkedListNode *)malloc( sizeof(LinkedListNode) );
        node->key = key;
        node->code = code;
        node->next = NULL;
        dict->llArray[key->hashValue] = node;
        dict->size++;
        return;
    }

    dict->collisions++;

    while( node != NULL ) {
        if( identicalSequences(key, node->key) ) {
            node->code = code;
            return;
        }
        prevNode = node;
        node = node->next;
    }

    node = (LinkedListNode *)malloc( sizeof(LinkedListNode) );
    node->key = key;
    node->code = code;
    node->next = NULL;

    prevNode->next = node;
    dict->size++;

    return;
}
