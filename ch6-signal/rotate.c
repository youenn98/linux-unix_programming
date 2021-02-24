#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

char conv[26];
int ter_len = 0;

void ttymode(int md){
    static struct termios status;
    static int ori_flags;
    if(md == 0){
        tcgetattr(0,&status);
        ori_flags = fcntl(0,F_GETFL);
    }else if(md == 1){
        tcsetattr(0,TCSANOW,&status);
        fcntl(0,F_SETFL,ori_flags);
    }
}

void print_back(){
    printf("\b \b");
}

void set_noecho(){
    struct termios s;
    tcgetattr(0,&s);
    s.c_lflag &= ~ICANON;
    s.c_lflag &= ~ECHO;
    s.c_cc[VMIN] = 1;
    s.c_cc[VTIME] = 0;
    tcsetattr(0,TCSANOW,&s);
}



int main(){
    int c;
    for(int i = 0; i < 26;i++) conv[i] = 'a' + i;
    ttymode(0);
    set_noecho();
    while((c = getchar()) != EOF){
        if(c == 'Q'){
            ttymode(1);
            return 0;
        }
        if(islower(c)){
            putchar(conv[(c-'a'+1)%26]);
            ter_len++;
        }
        if(c == 127){
            print_back();
            ter_len--;
        }
        if(c == 126){
            putchar('\r');
            for(int i = 0; i < ter_len;i++){
                putchar(' ');
            }
            putchar('\r');
        }
    }
    ttymode(1);
}