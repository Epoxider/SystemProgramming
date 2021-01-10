#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

int getAllStdin(char** buf) {
    printf("Reading in Stdin...\n");
    int buffer_size = 256;
    int total_bytes = 0;
    char * ptr = (char*)malloc(buffer_size);
    //asser(ptr != 0);
 
    int bytes_read = (0, ptr, buffer_size - total_bytes);
    printf("Read %d bytes \n", bytes_read);
    
    while(bytes_read > 0) {
        total_bytes += bytes_read;
        buffer_size += 256;
        ptr = (char*)realloc(ptr, buffer_size);
        bytes_read = read(0, &(ptr[total_bytes]), buffer_size - total_bytes);
        printf("Read %d bytes \n", bytes_read);
    }
    *buf = ptr;
    return total_bytes;
}


int main(int argc, char* argv[]) {
    int rd;
    char c;
    int position;
    char * buf = NULL;
    
    if(argc < 2) {
        int length = getAllStdin(&buf);
        for (int i = length; i >= 0; i--) {
            write(1,&(buf[i]),1);
        }
        free(buf);
    }
    else 
    {
        rd = open(argv[1], O_RDONLY);
        assert(rd > 0);
        position = lseek(rd, 0, SEEK_END);
        while(position >= 0 ) {
            read(rd,&c,1);
            write(1,&c,1);
            position = lseek(rd, -2, SEEK_CUR);
        }
    }
    close(rd);
    return 0;
}
    
