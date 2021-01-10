#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *left;
  struct node *right;
} Node;

void insert(Node **currentPtr, int val) {
  Node * current = *currentPtr;

  if(current == NULL) {
    *currentPtr = malloc(sizeof(Node));
    current = *currentPtr;

    current->value = val;
    current->left = NULL;
    current->right = NULL;
  } else if (val < current->value) {
      insert(&(current->left), val);
  } else {
      insert(&(current->right), val);
  }
}

void preOrder(Node *current) {
  if(current == NULL) {
    return;
  }
  printf("%d\n",node->value)
  preOrder(current->left);
  preOrder(current->right);
}

void inOrder(Node *current) {
  if(current == NULL) {
    return;
  }
  inOrder(node->left);
  printf("%d\n",node->value);
  inOrder(node->right);
}

void postOrder(Node *current) {
  if(current == NULL) {
    return;
  }
  postOrder(node->left);
  postOrder(node->right);
  printf("%d\n",node->value);
}
void freeTree(Node **currentPtr) {
}

int main() {
  Node *current = NULL;
  const int SIZE = 20000;
  int i;
  for(i = 0; i < SIZE; i++) {
    insert(&current, 0);
  }


  freeTree(&current);
}
