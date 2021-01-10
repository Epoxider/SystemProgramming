#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long hash(char *string_to_hash, int size)
{
    unsigned long hash = 5381;
    int c;

    while ( (c = *string_to_hash++) )
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % size;
}

int main(){
	unsigned long k=1;
        char *str = (char *) malloc( 256 * sizeof(char));
	if(str == (void *)0) {
	    printf("Couldn't allocate memory\n\n");
	    exit(-1);
	}

	strcpy(str, "Hello, World! StonksPoggers in the chat? also hit that like and follow to see more content");
	k = hash(str, 16384);
	printf("Hash for %s id %ld\n", str, k);

	str[2] = 'E';
	k = hash(str, 16384);
	printf("Hash for %s id %ld\n", str, k);
	free(str);

	str = (void *)0;



	return(0);
}
