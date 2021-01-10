#include <stdio.h>
#include <stdlib.h>
#include "binaryTree.h"

int main() {
  Node *current = NULL;
  int numFound;
  int n;
  int keepGoing = 1;
  char buf[255];
  /* Implemeted a while loop that takes a user imput and depending on the imput, steps into the respective
  case. terminates when user enters q.

  Each case that has a number argument, tests to see if number read is successful 
  and if not, do another scanf to consume invalid argument from input stream */

  while( keepGoing ) {
    numFound = scanf("%s", &(buf[0]));
    if( numFound < 1 )
      continue;

    switch(buf[0]) {
      case 'i':
        /* inserts the integer given by user into the tree */
        numFound = scanf("%d", &n);
        if( numFound < 1 ) {
          scanf("%s", &(buf[0]));
          continue;
        }

        insert(&current, n);
        break;

      case 'd':
        /* deletes the integer given by user from the tree*/
        numFound = scanf("%d", &n);
        if( numFound < 1 ) {
          scanf("%s", &(buf[0]));
          continue;
        }

        deleteVal( &current, n );
        break;

      case 's':
        /* searches for integer given by user*/
        numFound = scanf("%d", &n);
        if( numFound < 1 ) {
          scanf("%s", &(buf[0]));
          continue;
        }

        search(current, n);
        break;

      case 'e':
        /* emptys tree*/
        empty(&current);
        break;

      case 't':
        /* needed another switch statment cause of the 3 different ways to travel the tree */
        numFound = scanf("%s", &buf[0]);
        if( numFound < 1 )
          continue;

        switch(buf[0]) {
          case 'i':
            inOrder(current);
            break;

          case 'l':
            preOrder(current);
            break;

          case 'r':
            postOrder(current);
            break;

          default:
            break;
        }
        break;

      case 'q':
        /* this quits the while loop and exist program */
        keepGoing = 0;
        break;

      default:
        break;
    }


  }

  empty(&current);
}
