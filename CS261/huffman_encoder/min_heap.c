
#include <stdio.h>
#include <stdlib.h>
#include "min_heap.h"

#define PARENT(i) ((i)/2)
#define LEFT_CHILD(i) (2*(i))
#define RIGHT_CHILD(i) (2*(i)+1)

#define FRONT (1)

Heap * createHeap(int maxHeapSize) {
  int i;
  Heap * h = (Heap *)malloc( sizeof(Heap) );

  /* The first node lives at index 1, not zero.  Allocate 1 bigger. */
  h->nodes = (BinaryTree **)malloc( (maxHeapSize+1) * sizeof(BinaryTree*));
  for(i = 0; i < maxHeapSize; i++) {
    h->nodes[i] = NULL;
  }

  h->max = maxHeapSize;
  h->size = 0;

  return h;
}

void destroyHeap(Heap * h) {
  if( h != NULL ) {
    int i;
    for(i = 0; i < h->max; i++) {
      if( h->nodes[i] != NULL ) {
        destroyBinaryTree( h->nodes[i] );
        h->nodes[i] = NULL;
      }
    }
    
    free(h->nodes);
    free(h);
  }
}

void swap(Heap * h, int a, int b) {
  BinaryTree * tmp = h->nodes[a];
  h->nodes[a] = h->nodes[b];
  h->nodes[b] = tmp;
}

void insertNode( Heap * h, BinaryTree * bt ) {
  if( h->size >= h->max ) {
    printf("\n\nYIKES!!  Trying to add to full tree!!\n");
    exit(-1);
  }

  if( h != NULL ) {
    h->size++;
    h->nodes[h->size] = bt;
    int i = h->size;

    while( (PARENT(i) > 0) && (h->nodes[i]->count < h->nodes[PARENT(i)]->count) ) {
        swap(h, i, PARENT(i) );
        i = PARENT(i);
    }
  } 
}

BinaryTree * removeNode(Heap * h) {
  if( h->size == 0 ) {
    return NULL;
  }

  BinaryTree * bt = h->nodes[FRONT];
  if( h->size > 1 ) {
    h->nodes[FRONT] = h->nodes[h->size];
    h->nodes[h->size] = NULL;
    h->size--;
    minify(h, FRONT);
  } else {
    h->nodes[FRONT] = NULL;
    h->size = 0;
  }
  return bt;
}

int isLeafNode(Heap * h, int i) {
  return (i >= (h->size/2)) && (i <= h->size);
}

void minify(Heap * h, int i) {
  if( !isLeafNode(h, i) ) {
    if( (h->nodes[i]->count > h->nodes[LEFT_CHILD(i)]->count) || 
        (h->nodes[i]->count > h->nodes[RIGHT_CHILD(i)]->count) ) {
      if( h->nodes[LEFT_CHILD(i)]->count < h->nodes[RIGHT_CHILD(i)]->count) {
        swap( h, i, LEFT_CHILD(i) );
        minify( h, LEFT_CHILD(i) );
      } else {
        swap( h, i, RIGHT_CHILD(i) );
        minify( h, RIGHT_CHILD(i) );
      }
    }
  }
}

void printHeap(Heap * h) {
  int i;
  for( i = h->size/2; i >= 1; i-- ) {
    BinaryTree * N = h->nodes[i];
    BinaryTree * L = h->nodes[LEFT_CHILD(i)];
    if( RIGHT_CHILD(i) <= h->size ) {
      BinaryTree * R = h->nodes[RIGHT_CHILD(i)];
      printf(" N : [%d,%d], L : [%d,%d], R : [%d,%d]\n", N->value, N->count, L->value, L->count, R->value, R->count); 
    } else {
      printf(" N : [%d,%d], L : [%d,%d]\n", N->value, N->count, L->value, L->count);
    }
  }
}
