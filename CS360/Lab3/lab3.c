#include <unistd.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
    
    int fd[2];
    char c;
    int i = 1;

    if((argc > 2) && ((argc % 2) == 1)) {

    pipe (fd);
    
    if (fork()) {
        //Parent Process
        close (fd[0]);
        while(read(0,&c,1)) {
            write (fd[1], &c, 1);
        }
        close (fd[1]);
    } else {
        //Child Process
        close (fd[1]);
 
        while(read(fd[0],&c,1)) { 
            while(i < argc) {
                if (c == *argv[i]) {
                    c = *argv[i+1];
                    break;
                }
                i += 2; 
            }
            write(1,&c,1);
            i = 1;
        }
        close (fd[0]);
    }

    return 0;
 
    }
}

