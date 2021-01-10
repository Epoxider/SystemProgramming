#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handler(){
}

int main() {

    char c;
    struct sigaction SA;
    memset(&SA,'\0',sizeof(SA));

    SA.sa_handler = handler;
    sigaction(SIGALRM,&SA,NULL);
   
    while(read(0,&c,1)) {
        write(1,&c,1);      
        alarm(1);
        pause();
        alarm(0);
    }
}
