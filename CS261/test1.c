#include <stdio.h>
#include <stdlib.h>

int *create_array(int n, int iValue)
{
  int *ptr = (int*) malloc(n*sizeof(int));
  if(ptr!=NULL)
  {
    int i;
    for(i=0;i<n;i++)
    {
      ptr[i] = iValue;
    }
  }
  return ptr;
}
  
int main()
{
  int* arr = create_array(7,0);
  for(int i = 0; i < 7; i++)
  { 
    printf("%d\n", arr[i]);
  }
}
