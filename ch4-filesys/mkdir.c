#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)

int IGNORE = 0;
int PARENT = 0;
int parse_d(char dest[],char src[],int index){
    int i = 0;
    while(src[index] != '/' && src[index] != 0){
        dest[i] = src[index];
        i++;index++;
    }
    if(src[index] == '/') return index + 1;
    else return -1;


}
int main(int argc,char *argv[]){
    if(argc <= 1){
        perror("Enter directory name.\n");
        exit(1);
    }
    int all_argc = argc;
    while(argc > 2){
        if(strlen(argv[argc-2]) == 2){
            switch(argv[argc-2][1]){
                case 'p':
                    IGNORE = 1;
                    PARENT = 1;
                    break;
                default:
                    perror("Parameter Error");
                    printf("Can not resolve %s\n",argv[argc-2]);
                    exit(1);
            }
        }else{
            perror("Use Option -- ,example --p\n");
            exit(1);
        }
        argc--;
    }

    char d_name[1024];
    strcpy(d_name,argv[all_argc-1]);

    if(!PARENT){
        if(mkdir(d_name,MODE) == -1){
            perror("Fail to create directory :");
            exit(1);
        }
    }else{
        int index = 0;
        while (index != -1){   
            char temp_name[256];
            index = parse_d(temp_name,d_name,index);
            if(mkdir(temp_name,MODE) == -1){
                if(errno != EEXIST){
                     perror("Fail to create directory :");
                     exit(1);
                }
            }
            chdir(temp_name);
        }
    }



}