/*************************************************************************
	> File Name: utmplib.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: Tuesday, August 11, 2020 PM11:17:41 PDT
 ************************************************************************/

#include<stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define	UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];//storage
static int num_recs;//num_stored
static int cur_recs;//next
static int fd_utmp;//read from

utmp_open(char *filename){
	fd_utmp = open(filename,O_RDONLY);
	cur_recs = num_recs = 0;
	return fd_utmp;
}

struct utmp *utmp_next()
{
	struct utmp * recp;
	if(fd_utmp == -1) return NULLUT;
	if(cur_recs == num_recs && utmp_reload() == 0)
		return NULLUT;
	recp = (struct utmp *) &utmpbuf[cur_recs * UTSIZE];
	cur_recs++;
	return recp;
}	

int utmp_reload()
{
	int amt_read;
	amt_read = read(fd_utmp,utmpbuf,NRECS * UTSIZE);
	num_recs = amt_read/UTSIZE;
	cur_recs = 0;
	return num_recs;
}

void utmp_close(){
	if(fd_utmp != -1)
		close(fd_utmp);

}







