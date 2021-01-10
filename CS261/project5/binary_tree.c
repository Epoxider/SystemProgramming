#include "binary_tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void destroyBinaryTree(BinaryTree * bt) {
  if( bt != NULL ) {
    if( bt->left != NULL ) {
      destroyBinaryTree(bt->left);
    }
    if( bt->right != NULL ) {
      destroyBinaryTree(bt->right);
    }
    free(bt);
  }
}

void printTheTree(BinaryTree * bt, int depth, char * prefix ) {
  if( bt == NULL ) {
    return;
  }
  int i;
  for( i = 0; i < depth; i++) {
    printf("\t");
  }
  printf("%s-> value=%d, count=%d\n",prefix, bt->value, bt->count);
  printTheTree(bt->left, depth+1, "L");
  printTheTree(bt->right, depth+1, "R");
}

void printTree(BinaryTree * bt) {
  printTheTree(bt, 0, "");
}
