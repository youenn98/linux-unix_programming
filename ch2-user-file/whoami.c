/*************************************************************************
	> File Name: who.c
	> Author: Yao Yuan
	> Created Time: Friday, July 24, 2020 PM10:40:47 PDT	
	similate who instruction.
	print username:(pid)	login_date(ASCII)
	this program read the utmp file, get infomation of user.
************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<utmp.h>
#include	<fcntl.h>
#include	<time.h>

void show_time(long);
void show_info(struct utmp *);

int main(){
	struct utmp utmpbuf;
	int utmpfd;
	if( (utmpfd = open(UTMP_FILE,O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}
	size_t bufsz = sizeof(utmpbuf);

	while(read(utmpfd,&utmpbuf,bufsz) == bufsz){
		show_info(&utmpbuf);
	}

	if(close(utmpfd) == -1){
		perror(UTMP_FILE);
	}	
	exit(0);
}


void show_time(long timeval){
	char *t_buf;
	t_buf = ctime(&timeval);
	printf("%s",t_buf+4);
	
}
void show_info(struct utmp * utmpbufp){
	if(utmpbufp->ut_type != USER_PROCESS){
		return;
	}
	printf("%s",utmpbufp->ut_user);
	putchar('\n');
}
