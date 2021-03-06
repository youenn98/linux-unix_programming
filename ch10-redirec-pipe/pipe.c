#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define oops(m,x) {perror(m);exit(x);}

int main(int ac,char ** av){
    int thepipe[2],newfd,pid;

    if(ac != 3){
        fprintf(stderr,"usage: pipe cmd1 cmd2\n");
        exit(1);
    }
    if(pipe(thepipe) == -1)
        oops("Cannot get a pipe",1);
    //After get a pipe, create two proccesses

    if((pid = fork()) == -1)
        oops("Cannot fork",2);
    
    /*parent read from pipe*/
    if(pid > 0){
        close(thepipe[1]);

        if(dup2(thepipe[0],0) == -1)
            oops("could not redirect stdin",3);
        
        close(thepipe[0]);//stdin is duped, close pipe
        execlp(av[2],av[2],NULL);
        oops(av[2],4);
    }
    /*child execs av[1] and writes into pipe */
    close(thepipe[0]);
    if(dup2(thepipe[1],1) == -1)
        oops("could not redirect stdout",4);
    close(thepipe[1]);
    execlp(av[1],av[1],NULL);
    oops(av[1],5);
}