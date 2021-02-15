#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

int main(){
    struct termios info;
    int rv;

    rv = tcgetattr(0,&info);
    if(rv == -1){
        perror("tcgetattr");
        exit(1);
    }
    if(info.c_lflag & ECHO){
        printf("Echo is on\n");
    }else{
        printf("echo is OFF, since its bits is 0\n");
    }

}