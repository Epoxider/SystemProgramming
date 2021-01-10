#include <stdio.h>
#include <stdlib.h>


int lineLength(const char *filename, int n) {
  FILE * fp;
  int i;
  int k = 0;
  char ch;

  if( n < 1 ) {
    return k;
  }

  if ( (fp = fopen (filename, "r")) == NULL) {
    printf("file %s doesnt exist", filename);
    exit(-1);
  }
  for(i = 0; i < n-1; i++) {
    ch = fgetc(fp);
    while((ch != '\n') && (ch != EOF)) {
      ch = fgetc(fp);
    }
  }
  if (ch == EOF) {
    return k;
  }
  ch = fgetc(fp);
  while((ch != '\n') && (ch != EOF)) {
    ch = fgetc(fp);
    k++; 
  }
  return k;
}


int main(int argn, char ** argv) {
  if( argn != 3 ) {
    printf("Needs 2 parameters: 1) filename, 2) Line to measure\n");
    exit(-1);
  }

  int line = atoi(argv[2]);
  int length = lineLength(argv[1], line);
  printf("length = %d \n", length);
}
