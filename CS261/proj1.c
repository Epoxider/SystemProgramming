/* Matthew fritz 
 * CS 261
 * Project 1 "Fibonacci" */
#include <stdio.h>


/* fib function to be called*/
int fib(int n)
{
	int x = 0;
	int y = 1;
	int next;
	int i;
	for(i =  1; i <= n;i++)
	{
		next = x + y;
		x = y;
		y = next;
	}
	return x;
}

int main()
{
	int n;/* n will be number entered by user*/
	int a = 1;
	while(a)
	{
		printf("Value: ");
		scanf("%d",&n);
		if(n < 0) /*reprompt if the user enteres a num less than 0*/
		{
			continue;
		} else if(n > 0){ /* takes num user entered and prints the fib num associated*/
			printf("Fibonacci number %d is %d\n",n,fib(n));
		} else if(n == 0){/*ends program whenever user enters 0*/
			printf("Fibonacci number 0 is 0\n");
			return 0;

		}
	}

}


