#include "binaryTree.h"
#include <stdio.h>
#include <stdlib.h>

/*iteratively calls insert checking to see if current is null.  If not null,
checks if val is greater or less than current->value, calling insert with 
current->left or current->right respectivly */

void insert(Node **currentPtr, int val) {
  Node * current = *currentPtr;

  if(current == NULL) {
    *currentPtr = malloc(sizeof(Node));
    current = *currentPtr;

    if( current != NULL ) {
      current->value = val;
      current->left = NULL;
      current->right = NULL;
    }
  } else if (val <= current->value) {
      insert(&(current->left), val);
  } else {
      insert(&(current->right), val);
  }
}

/*compares val to current->value and, is greater or less than, calls search with 
current->right or current->left respectivly.  Once found, prints value found.
if not found, print missing message. */
void search(Node *current, int val) {
  if(current == NULL) {
    printf("%d is missing\n", val);
    return;
  }
  if( val == current->value ) {
    printf("%d is present\n", val);
  } else if ( val < current->value ) {
    search(current->left, val);
  } else {
    search(current->right, val);
  }
}

/* finds node to delete.  if no children, delete it.
if any children in left subtree, find largest value node, delete that and put
its value in the original node to delete.  If no left but has right subtree, find
smallest value node in that tree, delete that node and replace its value in the
original node to delete. */
void deleteVal(Node **currentPtr, int val) {
  if(*currentPtr == NULL) {
    return;
  }

  if( val == (*currentPtr)->value ) {
    /*if the deleted node has no children, just free it and set *currentPtr to null*/
    if( ((*currentPtr)->left == NULL) && ((*currentPtr)->right == NULL) ) {
      free(*currentPtr);
      *currentPtr = NULL;
    } else if( (*currentPtr)->left == NULL ) {
      /* Only right sub-tree */

      if( (*currentPtr)->right->left == NULL ) {
        /* The smallest value in the right subtree is just the first right node.  */
        Node * nodeToDelete = (*currentPtr)->right;
        (*currentPtr)->value = nodeToDelete->value;
        (*currentPtr)->right = nodeToDelete->right;
        free(nodeToDelete);
      } else {
        /* Right tree has some left nodes.  Find the left-most, and swap then delete. */
        Node * parent = (*currentPtr)->right;
        Node * nodeToDelete;
        while( parent->left->left != NULL ) {
          parent = parent->left;
        }
        nodeToDelete = parent->left;
        (*currentPtr)->value = nodeToDelete->value;
        parent->left = nodeToDelete->right;
        free( nodeToDelete );
      }
    } else {
      if( (*currentPtr)->left->right == NULL ) {
        /* The largest value in the left subtree is just the first left node.  */
        Node * nodeToDelete = (*currentPtr)->left;
        (*currentPtr)->value = nodeToDelete->value;
        (*currentPtr)->left = nodeToDelete->left;
        free(nodeToDelete);
      } else {
        /* Left tree has some right nodes.  Find the right-most, and swap then delete. */
        Node * parent = (*currentPtr)->left;
        Node * nodeToDelete;
        while( parent->right->right != NULL ) {
          parent = parent->right;
        }
        nodeToDelete = parent->right;
        (*currentPtr)->value = nodeToDelete->value;
        parent->right = nodeToDelete->left;
        free( nodeToDelete );
      }
    }
  } else if ( val < (*currentPtr)->value ) {
    deleteVal(&((*currentPtr)->left), val);
  } else {
    deleteVal(&((*currentPtr)->right), val);
  }
}

/*iterativly calls empty on itself freeing memory of *currentPtr
and setting *currentPTr to null*/
void empty(Node **currentPtr) {
  if(*currentPtr == NULL) {
    return;
  }
  empty(&((*currentPtr)->left));
  empty(&((*currentPtr)->right));
  free(*currentPtr);
  *currentPtr = NULL;
}

void preOrder(Node *current) {
  if(current == NULL) {
    return;
  }
  printf("%d\n",current->value);
  preOrder(current->left);
  preOrder(current->right);
}

void inOrder(Node *current) {
  if(current == NULL) {
    return;
  }
  inOrder(current->left);
  printf("%d\n",current->value);
  inOrder(current->right);
}

void postOrder(Node *current) {
  if(current == NULL) {
    return;
  }
  postOrder(current->left);
  postOrder(current->right);
  printf("%d\n",current->value);
}
