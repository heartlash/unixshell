#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<utime.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(int argc,char* argv[])
{
int fd,ret;
char *fname;
struct stat sb;
DIR *opendr= opendir(argv[1]);
chdir(argv[1]);
struct dirent* dirEnt=readdir(opendr);
while(dirEnt!=NULL)
{
printf("I-node no: %ld and name %s  and type %d \n",dirEnt->d_ino,dirEnt->d_name,dirEnt->d_type);
fname=dirEnt->d_name;
fd=open(fname,O_RDWR,0664);
ret=fstat(fd,&sb);
if(sb.st_size==0)
{
remove(fname);
}
dirEnt=readdir(opendr);
}
return 0;
}

