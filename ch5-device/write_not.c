#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(){
    char out_put[255] = " not";
    
    while(true){
        if(lock_test() == 0){
            FILE * f = fopen("test.txt","a");
            if(f != NULL){
                fwrite(out_put,sizeof(char),strlen(out_put),f);
                fclose(f);
                unlock_test();
                return 0;       
            }else{
                unlock_test();
                fprintf(stderr,"Fail to read test.txt\n");
                exit(1);
            }
        }
    }
    
}