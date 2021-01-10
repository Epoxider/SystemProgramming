#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


void handler(){
     printf("b\n");
}

int main() {

    struct sigaction SA;
    memset(&SA,'\0',sizeof(SA));

    SA.sa_handler = handler;
    sigaction(SIGINT,&SA,NULL);

    int flag = 1;
    while(flag == 1) {
    
        printf("%s\n","a");
        sleep(1);
    }
    return 0;
}
