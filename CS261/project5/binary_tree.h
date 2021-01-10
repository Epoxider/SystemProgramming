#ifndef BINARY_TREE_H
#define BINARY_TREE_H

typedef struct BinaryTree_struct {
  int value;
  int count;
  struct BinaryTree_struct * left;  
  struct BinaryTree_struct * right;  
} BinaryTree;

void destroyBinaryTree(BinaryTree * bt);
void printTree(BinaryTree * bt);

#endif
