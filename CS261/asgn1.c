#include <stdio.h>
#include <limits.h>
int main()
{
  int i =INT_MIN + 10;
  while(i)
  {
	  if(i == INT_MAX - 10) break;
    printf("T minus %d and counting\n", i--);
  }
  return 0;
}
