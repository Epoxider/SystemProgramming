/* Matthew Fritz
 * CS 261
 * Project 1 Fibonacci*/
#include <stdio.h>

/* fib function to be called*/
int fib(int n)
{
	int x = 0;
	int y = 1;
	int next;
	int i;
	for(i = 1; i <= n; i++)
	{ 
		next = x + y;
		x = y;
		y = next;
	}
	return x;
}

int main()
{
	int n; /* n will be num entered by user*/ 
	int a = 1;
	while(a)
	{
		printf("Value: ");
		scanf("%d",&n);
		if(n<0) /* reprints prompt if user enteres a num less than 0 */
		{ 
			continue;
		} else if(n>0){ /* prints wanted string and continues on with loop*/
			printf("Fibonacci number %d is %d\n",n,fib(n));
		} else if(n==0){ /* prints wanted string and exits */
			printf("Fibonacci number 0 is 0\n");
			return 0;
		}
	}
}
