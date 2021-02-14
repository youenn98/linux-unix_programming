/*************************************************************************
	> File Name: ls2.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: Thursday, August 13, 2020 AM08:36:33 PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<dirent.h>
#include<time.h>
#include<string.h>

#define max(A,B) ((A)>(B))?(A):(B)

struct print_data{
	char mode[11];
	char link_n[5];
	char uname[16];
	char gname[16];
	char  fsize[8];
	char tim[16];
	char fname[256];	
	unsigned int inode_num;
};

char re_dir[255][255];

void do_ls(char *);
void show_entry_info(struct print_data *);
void store_entry_info(char *,char *,struct print_data *);
void convert_mode(int,char[]);
char * uid_to_name(int);
char * gid_to_grp(int);
int comp(const void *,const void *);


int sz = 0;
int cur = 0;
int tail = 0;
int max_length[7];

int main(int arc,char *arv[]){


	if(arc == 1){
		do_ls(".");
	}
	else{
		while(--arc){
			printf("%s:\n",*(++arv));
			do_ls(*arv);
			while(cur != tail){
				printf("%s:\n",re_dir[cur]);
				do_ls(re_dir[cur++]);
			}
		}
		
	}
	return 0;
}

void do_ls(char * dirname){
	memset(max_length,0,sizeof(max_length));
	sz = 0;

	DIR * dirp;
	struct dirent * direntp;
	struct print_data * pdata = NULL;
	char path[256];

	if( (dirp = opendir(dirname)) == NULL){
		fprintf(stderr,"ls2: cannot open %s.\n",dirname);
		return;
	}else{
		while((direntp = readdir(dirp)) != NULL){
			sz++;
			pdata = realloc(pdata,sizeof(struct print_data) * sz);
			strcpy(path,dirname);
			strcat(path,"/");
			strcat(path,direntp->d_name);
			store_entry_info(path,direntp->d_name,&(pdata[sz-1]));
			
			if(direntp->d_type == DT_DIR && strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
				strcpy(re_dir[tail++],path);
			}
			
		}
		closedir(dirp);
	}
	show_entry_info(pdata);
	free(pdata);
}

void store_entry_info(char * path,char * fname,struct print_data * p){
	struct stat fstat; 
	if(stat(path,&fstat) == -1){
		fprintf(stderr,"connot get the stat of %s\n",fname);
		return;
	}
	char str[11];
	convert_mode(fstat.st_mode,str);

	p->inode_num = fstat.st_ino;

	//printf("%s\n",str);
	sprintf(p->mode,"%s",str);
	max_length[0] = max(max_length[0],strlen(str));

	sprintf(p->link_n,"%d",(int)fstat.st_nlink);
	max_length[1] = max(max_length[1],strlen(p->link_n));
	
	sprintf(p->uname,"%s",uid_to_name(fstat.st_uid));
	max_length[2] = max(max_length[2],strlen(p->uname));

	sprintf(p->gname,"%s",gid_to_grp(fstat.st_gid));
	max_length[3] = max(max_length[3],strlen(p->gname));

	sprintf(p->fsize,"%ld",(long)fstat.st_size);
	max_length[4] = max(max_length[4],strlen(p->fsize));

	sprintf(p->tim,"%.12s ",4+ctime((long int *)&fstat.st_ctim));
	max_length[5] = max(max_length[5],strlen(p->tim));

	sprintf(p->fname,"%s",fname);
	max_length[6] = max(max_length[6],strlen(p->fname));

}

void show_entry_info(struct print_data * p){

	qsort(p,sz,sizeof(struct print_data),comp);

	for(int i = 0;i < sz;i++){
			
		printf("%s ",p[i].mode);
		printf("%*s ",max_length[1],p[i].link_n);
		printf("%*s ",max_length[2],p[i].uname);
		printf("%*s ",max_length[3],p[i].gname);	
		printf("%*s ",max_length[4],p[i].fsize);
		printf("%*s ",max_length[5],p[i].tim);
		printf("%*s ",max_length[6],p[i].fname);
		printf("%8d\n",p[i].inode_num);
	}
}

void convert_mode(int st_mode,char str[]){
	strcpy(str,"----------");
	switch(st_mode & S_IFMT){
		case S_IFLNK://link file
			str[0]= 'l';
			break;
		case S_IFSOCK://socket
			str[0] = 's';
			break;
		case S_IFBLK://block device
			str[0] = 'b';
			break;
		case S_IFDIR://directory
			str[0] = 'd';
			break;
		case S_IFCHR://char device
			str[0] = 'c';
			break;
		case S_IFIFO://pipeline
			str[0] = 'p';
	}
	//use mask code to get info.
	if(st_mode & S_IRUSR) str[1] = 'r';
	if(st_mode & S_IWUSR) str[2] = 'w';
	if(st_mode & S_IXUSR) str[3] = 'x';
	
	if(st_mode & S_IRGRP) str[4] = 'r';
	if(st_mode & S_IWGRP) str[5] = 'w';
	if(st_mode & S_IXGRP) str[6] = 'x';
	
	if(st_mode & S_IROTH) str[7] = 'r';
	if(st_mode & S_IWOTH) str[8] = 'w';
	if(st_mode & S_IXOTH) str[9] = 'x';
	
	if(st_mode & S_ISUID){
		if(str[3] == 'x') str[3] = 's';
		else str[3] = 'S';
	}
	if(st_mode & S_ISGID){
		if(str[6] == 'x') str[6] = 's';
		else str[6] = 'S';
	}
}

char * uid_to_name(int uid){
	static char str[10];
	struct passwd * pw_ptr,* getpwuid();

	if( (pw_ptr = getpwuid(uid)) == NULL){
		sprintf(str,"%d",uid);
		return str;
	}
	else{
		return pw_ptr->pw_name;
	}
	
}
char * gid_to_grp(int gid){
	static char str[10];
	struct group * getgrgid(),* gr_ptr;
	
	if((gr_ptr = getgrgid(gid)) == NULL){
		sprintf(str,"%d",gid);
		return str;
	}else{
		return gr_ptr->gr_name;
	}
}

int comp(const void * a,const void * b){
	const struct print_data * aa = (const struct print_data *)a;
	const struct print_data * bb = (const struct print_data *)b;
	if(strcmp(aa->fname,bb->fname) <= 0) return 0;
	else return 1;
	
	
	
}
