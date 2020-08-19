/*************************************************************************
	> File Name: cp1.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: Wednesday, August 12, 2020 AM01:04:27 PDT
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>

#define BUFFERSIZE 4096
#define COPYMODE 0777

int copy_file(char *,char *);
int copy_dir(char *,char *);
int cut_slash(char *);

int cut_slash(char * dirname){
	int ret = -1;
	int i = 0;
	while(dirname[i]){
		if(dirname[i] == '/') ret = i;
		i++;
	}
	return ret;
}

int copy_file(char * sourcef,char * destinaf){
	
	int in_fd,out_fd,n_chars;
	char buff[BUFFERSIZE];

	if((in_fd = open(sourcef,O_RDONLY)) == -1){
		fprintf(stderr,"Cannot open ");
		perror(sourcef);
		return -1;
	}

	if((out_fd = creat(destinaf,COPYMODE)) == -1){
		fprintf(stderr,"Cannot open ");
		perror(destinaf);
		exit(1);
	}


	while((n_chars = read(in_fd,buff,BUFFERSIZE)) > 0){
		if(write(out_fd,buff,n_chars) != n_chars){
			fprintf(stderr,"Write error to ");
			perror(destinaf);
			return -1;
		}
	}	
	if(n_chars == -1) fprintf(stderr,"Read error from %s\n",sourcef);
	
	if(close(in_fd) == -1 || close(out_fd) == -1){
			fprintf(stderr,"Error close files.\n");
			return -1;
	}
	return 0;
}

int copy_dir(char * dirs,char * dird){
	char path[256];
	char fname[256];

	DIR * dirp;
	struct dirent * direntp;

	if((dirp = opendir(dirs)) == NULL){
		fprintf(stderr,"cannot open directory ");
		perror("path");
		return -1;
	}
	while((direntp = readdir(dirp)) != NULL){
		strcpy(fname,dirs);
		strcat(fname,"/");
		strcat(fname,direntp->d_name);
		
		printf("%s\n",fname);

		if(direntp->d_type & DT_DIR){

			if(strcmp(direntp->d_name,".")==0) continue;
			if(strcmp(direntp->d_name,"..")==0) continue;

			strcpy(path,dird);
			strcat(path,"/");
			strcat(path,fname + cut_slash(fname) + 1);
			mkdir(path,COPYMODE);
			copy_dir(fname , path);
		}
		else{
			strcpy(path,dird);
			strcat(path,"/");
			strcat(path,direntp->d_name);
			copy_file(fname,path);
		}
		
	}
	
	return 0;
	
}



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


	struct stat in_stat,out_state;
	char fname[256];
	

	if(stat(av[1],&in_stat) == -1){
		fprintf(stderr,"cannot get state of ");
		perror(av[1]);
		exit(1);
	}

	if(stat(av[2],&out_state) == -1){
		if(S_ISDIR(in_stat.st_mode)){
			mkdir(av[2],COPYMODE);
			copy_dir(av[1],av[2]);
		}else{
			copy_file(av[1],av[2]);
		}
		return 0;
	}
	
	if(S_ISDIR(in_stat.st_mode) && !S_ISDIR(out_state.st_mode)){
		fprintf(stderr,"source is directory, destination is file");
		exit(1);
	}
	
	if(!S_ISDIR(in_stat.st_mode) && !S_ISDIR(out_state.st_mode)){
		if(copy_file(av[1],av[2]) == -1){
			fprintf(stderr,"fail to copy from %s to %s\n",av[1],av[2]);
		}
	}

	if(!S_ISDIR(in_stat.st_mode) && S_ISDIR(out_state.st_mode)){
		strcpy(fname,av[2]);
		strcat(fname,"/");
		strcat(fname,av[1]);
		if(copy_file(av[1],fname) == -1){
			fprintf(stderr,"fail to copy from %s to %s\n",av[1],av[2]);
		}
	}

	if(S_ISDIR(in_stat.st_mode) && S_ISDIR(out_state.st_mode)){
		strcpy(fname,av[2]);
		strcat(fname,"/");
		strcat(fname,av[1] + cut_slash(av[1]) + 1);	
		mkdir(fname,COPYMODE);
		if(copy_dir(av[1],fname) == -1){
			fprintf(stderr,"fail to copy from %s to %s\n",av[1],av[2]);
		}
		
	}
}
