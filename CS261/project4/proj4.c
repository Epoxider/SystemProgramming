#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  char ch;
  FILE * fp;

  /* Check that the user handed in 2 (argc == 3) */
  if( argc != 3 ) {
    printf("\n%s should be called with 2 command line arguments\n", argv[0] );
    exit(-1);
  }

  /* Convert key to an integer */
  int key = atoi( argv[2] );

  while( key < 0 ) {
    key += 26;
  }

  fp = fopen(argv[1], "r");

  /* Check if file open was successful */
  if( fp == NULL ) {
    printf("\nCould not open %s for reading\n", argv[1] );
    exit(-1);
  }

  ch = fgetc(fp);
  while( ch != EOF ) {
    if( ch >= 'a' && ch <= 'z') {
      ch = ch + key;
      while( ch < 'a' ) {
        ch += 26;
      }
      while( ch > 'z' ) {
        ch -= 26;
      }
      printf("%c", ch);
    }
    else if (ch >= 'A' && ch <= 'Z') {
      ch = ch + key;
      while( ch < 'A' ) {
        ch += 26;
      }
      while( ch > 'Z' ) {
        ch -= 26;
      }
      printf("%c", ch);
    }
    else {
      printf("%c", ch);
    }
    ch = fgetc(fp);
  }
  fclose(fp);
}
