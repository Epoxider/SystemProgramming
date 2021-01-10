#include <stdlib.h>
#include <stdio.h>
#include "huffman_decoder.h"
#include "binary_tree.h"

void insertNode(BinaryTree * root, BinaryTree * node ) {

  /* If root is null, we have a problem */
  if (root == NULL) {
    printf("We have a problem, no root");
    return;
  }
  /* Now, walk the tree, reursively until you find a place to put the node */

  /* See if new node should be in left or right subtree */
  if(node->count <= root->count) {
    /* If left node is empty, just put it there */
    if(root->left == NULL) {
      root->left = node;
    } else {
      /* Else, recursively call insert */
      insertNode(root->left, node);
    }
  } else {
    /* If right node is empty, just put it there */
    if(root->right == NULL) {
      root->right = node;
    } else {
      /* Else, recursively call insert */
      insertNode(root->right, node);
    }
  }
      
}

/* Reads from stdin and constructs Huffman tree */
BinaryTree * buildTree() {
  int i, numNodes;
  BinaryTree * root = NULL;

  /* Read the first integer, which is the number of nodes */
  if( scanf("%d", &numNodes) < 1 ) {
    printf("Could not read number of tree nodes\n");
    return NULL;
  }

  /* Now read sets of two numbers, one for each huffman tree node */  
  for(i = 0; i < numNodes; i++) {
    int value, order;
    /* Read 2 numbers */
    int numRead = scanf("%d %d", &value, &order);
    if(numRead < 2) {
      printf("Could not read pair of tree node values\n");
      return NULL;
    }

    /* Create node, and assign values */
    BinaryTree* node = (BinaryTree*) malloc(sizeof(BinaryTree));
    if(node == NULL) {
      printf("node was null \n");
      exit(-1);
    }
    node->value = value;
    node->count = order;
    node->left = NULL;
    node->right = NULL;

    /* Now, add this node to the tree */
    if(root == NULL) {
      root = node;
    } else {
      insertNode(root, node);
    }
  }
  return root;
}

void decodeHuffman(BinaryTree * root) {
  int c;
  c = getchar();
  BinaryTree * bt = root;

  /* Keep reading while we have either '1' or '0' */
  while( c != '2' ) {
    /* If we got something else, ignore it */
    if( (c != '1') && (c != '0') ) {
      c = getchar();
      continue;
    }

    /* If '0', this means left sub-tree */
    if(c == '0') {
      /* Move down one level */
      bt = bt->left;
      /* If this is a leaf node, we have found our character */
      if( (bt->left == NULL) && (bt->right == NULL) ) {
        /* print it */
        printf("%c", bt->value);
        /* Reset back to root */
        bt = root;
      }
    } else {
      /* Move down one level */
      bt = bt->right;
      /* If this is a leaf node, we have found our character */
      if( (bt->left == NULL) && (bt->right ==NULL) ) {
        /* print it */
        printf("%c", bt->value);
        /* Reset back to root */
        bt = root;
      }
    }
    /* Get next character */
    c = getchar();
  }
  printf("\n");
}
