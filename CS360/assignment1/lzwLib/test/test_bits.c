#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

unsigned int getBit(unsigned int num, unsigned int bitPosition) {
   unsigned int bitStatus = (num >> bitPosition) & 1;
   return bitStatus;
}

int main() {
    unsigned int data = 30;
    for(int i = 0; i < 8; i++ ) {
        printf("The %d bit in %d (0x%08X) is set to %d \n", i, data, data, getBit(data,i));
    }
    return 0;

}
