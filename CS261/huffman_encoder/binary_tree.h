#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>

typedef struct BinaryTree_struct {
  int value;
  int count;
  struct BinaryTree_struct * left;  
  struct BinaryTree_struct * right;  
} BinaryTree;

BinaryTree * createBinaryTree(int value, int count);
void destroyBinaryTree(BinaryTree * bt);
void printTree(BinaryTree * bt);
int sizeTree(BinaryTree * bt);
void dumpBinaryTree(FILE * fp, BinaryTree * bt, int previousSize);

#endif
