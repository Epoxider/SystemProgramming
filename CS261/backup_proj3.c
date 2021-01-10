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

void travel(Node *current) {
  if(current == NULL) {
    return;
  }
  travel(current->left);
  travel(current->right);
}

void empty(Node **currentPtr) {
}

int main() {
  Node *current = NULL;
  int numFound;
  int n;
  int keepGoing = 1;
  char buf[255];

  while( keepGoing ) {
    numFound = scanf("%s", &(buf[0]));
    if( numFound < 1 )
      continue;

    switch(buf[0]) {
      case 'i':
        numFound = scanf("%d", &n);
        if( numFound < 1 )
          continue;

        printf("About to insert %d\n", n);
        break;

      case 'd':
        numFound = scanf("%d", &n);
        if( numFound < 1 )
          continue;

        printf("About to delete %d\n", n);
        break;

      case 's':
        numFound = scanf("%d", &n);
        if( numFound < 1 )
          continue;

        printf("About to search for %d\n", n);
        break;

      case 'e':
        empty(&current);
        break;

      case 't':
        numFound = scanf("%s", &buf[0]);
        if( numFound < 1 )
          continue;

        switch(buf[0]) {
          case 'i':
            printf("Traverse in-order\n");
            break;

          case 'l':
            printf("Traverse pre-order\n");
            break;

          case 'r':
            printf("Traverse post-order\n");
            break;

          default:
            break;
        }
        break;

      case 'q':
        keepGoing = 0;
        break;

      default:
        break;
    }


  }

  empty(&current);
}
