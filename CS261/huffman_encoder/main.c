#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "min_heap.h"
#include "binary_tree.h"

#define HISTOGRAM_SIZE (256)

void computeHistogram(char * filename, int * hist, int histSize) {
  FILE * fp = fopen(filename, "r");
  if( fp == NULL ) {
    printf("\nCould not open '%s' for reading\n\n", filename);
    exit(-1);
  }

  int charPos = 0;
  int c = fgetc(fp);
  while( c != EOF ) {
    if( c >= histSize ) {
      printf("Found character larger than histSize=%d: c=%d ad pos=%d\n", histSize, c, charPos);
      exit(-1);
    }
    if( (c >= 0) && (c < histSize) ) {
      hist[c]++;  
    }

    c = fgetc(fp);
    charPos++;
  }

  int i;
  for( i = 0; i < histSize; i++) {
    if( hist[i] > 0 ) {
      printf("%c [%d] : %d\n", (char)i, i, hist[i]);
    }
  }
}

BinaryTree * makeHuffmanTree(int * hist, int histSize) {
  int i;
  Heap * heap = createHeap(256);
  BinaryTree * bt;
 
  for( i = 0; i < histSize; i++ ) {
    if( hist[i] > 0 ) {
      BinaryTree * treeNode = createBinaryTree(i, hist[i]);
      insertNode(heap, treeNode);
    }
  }

  while(1) {
    bt = removeNode(heap);
    if( bt == NULL ) {
      printf("ACK!! Heap empty! Exiting\n");
      return NULL;
    }

    BinaryTree * rightTree = removeNode(heap);
    if( rightTree == NULL ) {
      destroyHeap(heap);
      return bt;
    }

    BinaryTree * parent = createBinaryTree(-1, bt->count + rightTree->count);
    parent->left = bt;
    parent->right = rightTree;

    insertNode( heap, parent );
  }
}

void populateHuffmanMap(BinaryTree * bt, char ** huffMap, int huffMapSize, char * code) {
  if( bt == NULL ) {
    printf("We should never get a tree that is NULL, since the bottom non-NULL leaf node should cause return\n");
    return;
  }
  if( bt->value >= 0 ) {
    char * s = (char *)malloc(strlen(code)*sizeof(char));
    strcpy(s, code);
    if( huffMap[bt->value] != NULL ) {
      printf("ACK!!! Already had huffMap entry for value=%d\n", bt->value);
      free(s);
      return;
    }
    huffMap[bt->value] = s;
  } else {
    /* Walk left tree */
    char * s = (char *)malloc((strlen(code)+2)*sizeof(char));
    strcpy(s, code);
    s[strlen(code)] = '0';
    s[strlen(code)+1] = 0;  /* NULL terminator */

    populateHuffmanMap(bt->left, huffMap, huffMapSize, s);
    
    /* Walk right tree */
    s[strlen(code)] = '1';
    populateHuffmanMap(bt->right, huffMap, huffMapSize, s);

    free(s);
  }
  
}

void printHuffmanMap( char ** huffMap, int huffMapSize) {
  int i;
  for( i = 0; i < huffMapSize; i++) {
    if( huffMap[i] != NULL ) {
      printf("%c [%d] -> %s\n", (i >= 32) ? (char)i : '?', i, huffMap[i] );
    }
  }
}

void huffmanEncode(char * inputFilename, char * outputFilename, char ** huffMap, int histSize, BinaryTree * bt) {
  FILE * fpOut = fopen(outputFilename, "w");
  if( fpOut == NULL ) {
    printf("\nCould not open '%s' for writing\n\n", outputFilename);
    exit(-1);
  }

  FILE * fpIn = fopen(inputFilename, "r");
  if( fpIn == NULL ) {
    fclose(fpOut);
    printf("\nCould not open '%s' for reading\n\n", inputFilename);
    exit(-1);
  }

  fprintf(fpOut, "%d\n", sizeTree(bt));
  dumpBinaryTree(fpOut, bt, 0);
  fprintf(fpOut, "\n");

  int c = fgetc(fpIn);
  while( c != EOF ) {
    fprintf(fpOut, "%s", huffMap[c]);
    c = fgetc(fpIn);
  }
  fprintf(fpOut, "\n");

  fclose(fpIn);
  fclose(fpOut);

}

int main(int argc, char *argv[]) {
  if( argc != 3 ) {
    printf("\nRequires 2 arguments: <input filename> <output filename>\n\n");
    exit(-1);
  }

  int hist[HISTOGRAM_SIZE];
  char * huffMap[HISTOGRAM_SIZE];
  int i;
  for( i = 0; i < HISTOGRAM_SIZE; i++) {
    hist[i] = 0;
    huffMap[i] = NULL;
  }

  computeHistogram(argv[1], hist, HISTOGRAM_SIZE);

  BinaryTree * bt = makeHuffmanTree(hist, HISTOGRAM_SIZE);

  printTree(bt);
  printf("Huffman binary tree size = %d\n", sizeTree( bt ) );

  populateHuffmanMap(bt, &(huffMap[0]), HISTOGRAM_SIZE, "");

  printHuffmanMap( &(huffMap[0]), HISTOGRAM_SIZE);

  huffmanEncode(argv[1], argv[2], &(huffMap[0]), HISTOGRAM_SIZE, bt);

  for( i = 0; i < HISTOGRAM_SIZE; i++) {
    if( huffMap[i] != NULL ) {
        free(huffMap[i]);
        huffMap[i] = NULL;
    }
  }

  destroyBinaryTree(bt);
  
}
