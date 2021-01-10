#include "binary_tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


BinaryTree * createBinaryTree(int value, int count) {
  BinaryTree * bt = (BinaryTree *)malloc(sizeof(BinaryTree));
  bt->value = value;
  bt->count = count;
  bt->left = NULL;
  bt->right = NULL;

  return bt;
}

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
  if( strlen(prefix) > 0 ) {
    printf("%s -> value=%c [%d], count=%d\n", prefix, (bt->value >= 32) ? (char)bt->value : '?', bt->value, bt->count);
  } else {
    printf("value=%c [%d], count=%d\n", (bt->value >= 32) ? (char)bt->value : '?', bt->value, bt->count);
  }
  printTheTree(bt->left, depth+1, "L");
  printTheTree(bt->right, depth+1, "R");
}

void printTree(BinaryTree * bt) {
  printTheTree(bt, 0, "");
}

int sizeTree(BinaryTree * bt ) {
  int size = 0;
  if( bt != NULL ) {
    size++;
    size += sizeTree(bt->left);
    size += sizeTree(bt->right);
  }
  return size;
}

void dumpBinaryTree(FILE * fp, BinaryTree * bt, int previousSize) {
  if( bt == NULL ) {
    return;
  }

  int leftPlusMe = sizeTree(bt->left) + 1;
  fprintf(fp, "%d %d ", bt->value, leftPlusMe + previousSize);

  dumpBinaryTree(fp, bt->left, previousSize);
  dumpBinaryTree(fp, bt->right, previousSize + leftPlusMe);
}
