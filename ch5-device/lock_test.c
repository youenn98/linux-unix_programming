#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

int unlock_test(){
    return unlink("test.txt.LCK");
}

int lock_test(){
    int rv = 0;
    struct stat lock_stat;
    if(link("test.txt","test.txt.LCK") == -1){
        rv = (errno == EEXIST?1:2);
    }
    
    if(rv == 1){
        if(stat("test.txt.LCK",&lock_stat) != 0){
            return 2;
        }
        struct timespec m_time = lock_stat.st_mtim;
        struct timespec now_time;
        clock_gettime(CLOCK_REALTIME,&now_time);

        //printf("now_time.tv_sec - m_time.tv_sec : %ld",now_time.tv_sec - m_time.tv_sec);

        if(now_time.tv_sec - m_time.tv_sec >= 10){
            unlock_test();
            return rv;
        }
    }
    return rv;
}
