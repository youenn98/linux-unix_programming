/*************************************************************************
	> File Name: cp1.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: Wednesday, August 12, 2020 AM01:04:27 PDT
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

int main(int ac,char *av[]){
	int in_fd,out_fd,n_chars;
	char buff[BUFFERSIZE];
	
	if(ac != 3){
		fprintf(stderr,"usage: %s source destination\n",*av);
		exit(1);
	}
	
	if(strcmp(av[1],av[2]) == 0){
		fprintf(stderr,"source file is same as destination file.\n");
		exit(1);
	}

	if((in_fd = open(av[1],O_RDONLY)) == -1){
		fprintf(stderr,"Cannot open ");
		perror(av[1]);
		exit(1);
	}
	
	if((out_fd = creat(av[2],COPYMODE)) == -1){
		fprintf(stderr,"Cannot open ");
		perror(av[2]);
		exit(1);
	}
	while((n_chars = read(in_fd,buff,BUFFERSIZE)) > 0){
		if(write(out_fd,buff,n_chars) != n_chars){
			fprintf(stderr,"Write error to ");
			perror(av[2]);
			exit(1);
		}
	}	
	if(n_chars == -1) fprintf(stderr,"Read error from %s",av[1]);
	
	if(close(in_fd) == -1 || close(out_fd) == -1){
			fprintf(stderr,"Error close files.\n");
			exit(1);
	}

}
