#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int main(){
    char out_put[][255] = {"waseda"," is"," best!\n"};
    struct timespec st_time;
    clock_gettime(CLOCK_REALTIME,&st_time);

    for(int i = 0;i < 3;i++){
        if(lock_test() == 0){
            FILE * f = fopen("test.txt","a");
            if(f != NULL){
                fwrite(out_put[i],sizeof(char),strlen(out_put[i]),f);
                fclose(f);
                struct timespec tt;
                clock_gettime(CLOCK_REALTIME,&tt);
                printf("%ld\n",tt.tv_sec-st_time.tv_sec);
                if(i != 2) sleep(6*(i+1));
            }else{
                unlock_test();
                fprintf(stderr,"Fail to read test.txt\n");
                exit(1);
            }
        }else{
            i--;
        }
    }
    unlock_test();
}