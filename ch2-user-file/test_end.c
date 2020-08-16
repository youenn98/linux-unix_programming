/*************************************************************************
	> File Name: test_end.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: Wednesday, August 12, 2020 AM12:32:05 PDT
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<string.h>

int main(){
	
	char buff[15] = "Hello World";

	int fd_test;
	fd_test = open("test.txt",O_WRONLY);
	lseek(fd_test,20000,SEEK_END);
	write(fd_test,buff,strlen(buff));
	close(fd_test);
	
	
	return 0;
}
