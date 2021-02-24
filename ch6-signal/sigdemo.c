#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


void f(int signum){
    static int count = 0;
    count++;
    printf("Interrupted! OK to quit(y/n)?");
    sleep(1);
    char c = getchar();
    if(c == 'y'){
        exit(0);
    }else if(c == 'n'){
        return;
    }
}

int main(int argc,char *argv[]){
    int i;
    signal(SIGINT,f);
    for(int i = 0; i < 5;i++){
        printf("hello\n");
        sleep(1);
    }
}