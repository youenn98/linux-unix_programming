/*************************************************************************
	> File Name: read_file.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: Sunday, August 16, 2020 AM05:11:25 PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(){
	int fd;
	fd = open("test.txt",O_RDONLY);
	char buff[256];
	memset(buff,0,sizeof(buff));
	if(read(fd,buff,12*sizeof(char)) == -1){
		perror("read error");
	}
	printf("%s\n",buff);
	
	memset(buff,0,sizeof(buff));
	sleep(10);
	if(read(fd,buff,12) == -1){
		perror("read error:");
	}
	printf("%s\n",buff);
	return 0;
}
