/*************************************************************************
	> File Name: chown1.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: Sunday, August 16, 2020 AM05:53:49 PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<ctype.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>

struct ID{
	int uid;
	int gid;
};

struct ID get_id(char *);
int get_gp(char *);
int get_ud(char *);

int main(int argc,char *argv[]){
	if(argc < 3){
		fprintf(stderr,"too few aruguments.\n");
		exit(1);
	}

	struct ID id = get_id(*(++argv));
	argc--;
	while(--argc){
		++argv;	
		//printf("%d %d\n",id.uid,id.gid);
		if(chown(*argv,id.uid,id.gid) == -1){
			perror("change ownership error: ");
		}
	}

	
	return 0;
}

struct ID get_id(char * arg){
	struct ID ret;
	int len = strlen(arg);
	int split;
	for(split  = 0; split < len;split++){
		if(arg[split] == ':') break;
	}
	if(split == 0) {
		ret.uid = -1;
		ret.gid = get_gp(arg+1);
	}
	else if(split == len){
		ret.gid = -1;
		ret.uid = get_ud(arg);
	}
	else{
		arg[split] = '\0';
		ret.uid = get_ud(arg);
		ret.gid = get_gp(arg + split + 1);
	}
	return ret;
}

int get_ud(char * str){
	if(isdigit(str[0])){
		return(atoi(str));
	}
	else{
		struct passwd * pp = getpwnam(str);
		return pp->pw_uid;
	}
}

int get_gp(char * str){
	
	if(isdigit(str[0])){
		return(atoi(str));
	}
	else{
		struct group * gp = getgrnam(str);
		return gp->gr_gid;
	}
}

