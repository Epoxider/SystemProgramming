#include <stdio.h>
#include <stdlib.h>

#include "huffman_decoder.h"
#include "binary_tree.h"

int main(int argc, char *argv[]) {
  BinaryTree * bt = buildTree();
  /* printTree(bt); */
  decodeHuffman(bt);
  destroyBinaryTree(bt);
}
