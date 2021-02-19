#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MB (1024*1024)
#define KB (1024)

int main(){

    int fl;
    int fd = open("100Mblock",O_CREAT|O_WRONLY);
    fl = fcntl(fd,F_GETFL);
    fl |= O_SYNC;
    fcntl(fd,F_SETFL,fl);
    char buff[1024];
    long int begintime = clock();
    

    for(int i = 0; i < 1024;i++) buff[i] = 'a' + i%26;
    for(int i = 0; i < 100;i++){
        for(int j = 0;j < 1024;j++){
            write(fd,buff,sizeof(buff));
        }
    }
    long int endtime = clock();
    printf("%lf ms\n",(endtime - begintime)/(double)CLOCKS_PER_SEC);
    close(fd);
}