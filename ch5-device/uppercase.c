#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>


int main(){
    struct termios info;
    int result = tcgetattr(0,&info);
    if(result == -1){
        perror("tcgetattr");
        exit(1);
    }
    if(info.c_oflag & OLCUC){
        printf("OLCUC flag is on.\n");
    }else{
        printf("OLCUC flag is off.\n");
    }
    return 0;
}