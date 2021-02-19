#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include <string.h>

int main(int argc,char *argv[]){

    char tty_list[256][256];
    int fd_list[256];
    char buf[BUFSIZ];
    int tty_cnt = 0;


    /*check args*/
    if(argc != 2 && argc != 3){
        fprintf(stderr,"usage:sudo ./write username [ttyname]\n");
        exit(1);
    }
    /*open utmp file to read login list*/
    struct utmp * utmpbufp,* utmp_next();
    if( utmp_open(UTMP_FILE) == -1){
		perror(UTMP_FILE);
		exit(1);
	}

	while((utmpbufp = utmp_next()) != ((struct utmp *)NULL)){
        if( strcmp(utmpbufp->ut_user,argv[1]) == 0){
            strcpy(tty_list[tty_cnt],"/dev/");

            if(utmpbufp->ut_line[0] == ":"){
                char tty_num[10];
                char temp[256];
                strcpy(temp,"pts/");
                sscanf(utmpbufp->ut_line,":%s",tty_num);
                strcat(temp,tty_num);
                strcat(tty_list[tty_cnt],temp);
            }else{
                strcat(tty_list[tty_cnt],utmpbufp->ut_line);
            }
            tty_cnt++;
        }
	}

    if(tty_cnt == 0){
        fprintf(stderr,"user %s is not login.\n",argv[1]);
        exit(1);
    }
    
    int has_open = 0;
    /* open device */
    for(int i = 0; i < tty_cnt;i++){
        if(argc == 3){
            if((strcmp(argv[2],tty_list[i]) != 0)&&(strcmp(argv[2],tty_list[i] + 5) != 0)){
                fd_list[i] = -1;
                continue;
            }
        }
        fd_list[i] = open(tty_list[i],O_WRONLY);
        if(fd_list[i] != -1){
            has_open = 1;
        }
    }

    if(!has_open){
        fprintf(stderr,"%s is not available.\n",argv[2]);
        exit(1);
    }


    /* loop until EOF on input */
    while(fgets(buf,BUFSIZ,stdin)!= NULL){
        int ok = 0;
        for(int i = 0; i < tty_cnt;i++){
            if(fd_list[i] != -1)
                if(write(fd_list[i],buf,strlen(buf)) != -1) ok = 1;
        }
        if(!ok) break;
    }

}