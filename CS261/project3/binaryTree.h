#ifndef BINARYTREE_H
#define BINARYTREE_H

typedef struct node {
  int value;
  struct node *left;
  struct node *right;
} Node;

void insert(Node **currentPtr, int val);
void search(Node *current, int val);
void deleteVal(Node **currentPtr, int val);
void empty(Node **currentPtr);
void inOrder(Node *current);
void preOrder(Node *current);
void postOrder(Node *current);

#endif 
