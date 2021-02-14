#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/fs.h>

int main(int argc, char *argv[]){ 
    if(argc != 3){
        perror("Use mv filename dirname/filename.");
        exit(1);
    }
    struct stat to_stat;
    
    int fd = open(argv[2],O_RDONLY);
    char old_path[256];
    char new_path[256];


    if(fd == -1){
        memcpy(old_path,argv[1],strlen(argv[1]));
        memcpy(new_path,argv[2],strlen(argv[2]));
    }else{
        fstat(fd,&to_stat);
        if(S_ISDIR(to_stat.st_mode)){
            memcpy(old_path,argv[1],strlen(argv[1]));
            memcpy(new_path,argv[2],strlen(argv[2]));
            strcat(new_path,"/");
            strcat(new_path,old_path);
        }else{
            memcpy(old_path,argv[1],strlen(argv[1]));
            memcpy(new_path,argv[2],strlen(argv[2]));
        }
    }

    if(rename(old_path,new_path) == -1){
        perror("Move failed : ");
        exit(1);
    }

    return 0;
}