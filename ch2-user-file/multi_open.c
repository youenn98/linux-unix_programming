/*************************************************************************
	> File Name: multi_open.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: Wednesday, August 12, 2020 AM12:44:56 PDT
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){
	
	char str[20] = "testing 123...";
	char buff[20];

	int fd_rd1 = open("test2.txt",O_RDONLY);
	int fd_wr1 = open("test2.txt",O_WRONLY);
	int fd_rd2 = open("test2.txt",O_RDONLY);
	printf("fd_rd1:%d ,fd_wr1:%d,fd_rd2:%d\n",fd_rd1,fd_wr1,fd_rd2);
	
	read(fd_rd1,buff,20);
	printf("%s\n",buff);
	write(fd_wr1,str,strlen(str));
	read(fd_rd2,buff,20);
	printf("%s\n",buff);

	return 0;
}
