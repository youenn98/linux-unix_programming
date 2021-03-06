#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    void countdown(int);

    signal(SIGALRM,countdown);
    if(set_ticker(500) == -1)
        perror("set ticker");
    else
        while (1)
        {
            pause();
        }
    return 0;
}

void countdown(int signum)
{
    static int num = 10;
    printf("%d..",num--);
    fflush(stdout);
    if(num < 0){
        printf("DONE!\n");
        exit(0);
    }

}