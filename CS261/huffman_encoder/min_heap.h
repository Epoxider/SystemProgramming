#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "binary_tree.h"

typedef struct {
    /* Array of BinaryTree pointers  */
    BinaryTree ** nodes;
    int size;
    int max;
} Heap;

Heap * createHeap(int maxHeapSize);
void destroyHeap(Heap * h);
void insertNode(Heap * h, BinaryTree * bt);
BinaryTree * removeNode(Heap * h);
int isLeafNode(Heap * h, int i);
void minify(Heap * h, int i);
void printHeap(Heap * h);

#endif
