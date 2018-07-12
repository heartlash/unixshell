#include<stdio.h>// reads a string from an input file and print the rvs of that in another txt file
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<utime.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc,char *argv[])
{
int fd1,fd2;
char buf[1];
int off;
fd1=open(argv[1],O_CREAT|O_RDWR,0644);
fd2=open(argv[2],O_CREAT|O_RDWR|O_TRUNC,0644);
if(fd1<0)
{
printf("file abc open error");
}
if(fd2<0)
{
printf("file rvs open error");
}
off=lseek(fd1,-2*sizeof(char),SEEK_END);
//printf("offset is %d\n",off);
/*read(fd1,buf,1*sizeof(char));
printf("buffer is %s",buf);*/
while(off!=-1)
{
read(fd1,buf,1*sizeof(char));
//printf("buffer is %s\n",buf);
write(fd2,buf,1);
off=lseek(fd1,-2*sizeof(char),SEEK_CUR);
//printf("offset is %d\n",off);
}
close(fd1);
close(fd2);
}


