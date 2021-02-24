#include <stdio.h>
#include <signal.h>
#include <termios.h>

int main(){
    struct termios status;
    struct termios st2;
    
    tcgetattr(0,&status);
    tcgetattr(0,&st2);
    /*
    if(status.c_lflag & ISIG){
        printf("ISIG is open\n");
    }else{
        printf("ISIG is close\n");
    }
    */
    st2.c_lflag &= ~ISIG;
    tcsetattr(0,TCSANOW,&st2);
    //signal(SIGINT,SIG_IGN);
    printf("you can't stop me! \n");
    while(1){
        sleep(1);
        printf("haha\n");
    }
    tcsetattr(0,TCSANOW,&status);
}