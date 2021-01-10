#include <stdio.h>

int stringgt(char * s1, char * s2);

#define MAX_SIZE (200)
int main(int argc, char ** argv) {
    char s1[MAX_SIZE];
    char s2[MAX_SIZE];

    printf("\nEnter first string: ");
    fgets(s1, MAX_SIZE-1, stdin);
    printf("Enter second string: ");
    fgets(s2, MAX_SIZE-1, stdin);

    int isGreater = 0;
    isGreater = stringgt(s1, s2);

    if( isGreater ) {
        printf("True\n");
    } else {
        printf("False\n");
    }
    return isGreater;
}
