#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//未优化版
int main()
{
  // 声明两个管道，用于父子进程进行收发
  int ffd[2];
  int sfd[2];
  pipe(ffd);
  pipe(sfd);
  if (fork()>0)
  {
    // 父进程
    char buffer[10]="ping";
    write(sfd[1],buffer,sizeof(buffer));
    close(sfd[1]);
    char readb[10];
    memset(readb,0,sizeof(readb));
    read(ffd[0],readb,sizeof(readb));
    printf("%d: received %s\n",getpid(),readb);
    close(ffd[0]);
  }
  else
  {
    // 子进程
    char buffer[10];
    memset(buffer,0,sizeof(buffer));
    read(sfd[0],buffer,sizeof(buffer));
    printf("%d: received %s\n",getpid(),buffer);
    char writeb[10]="pong";
    close(sfd[0]);
    write(ffd[1],writeb,sizeof(writeb));
    close(ffd[1]);
  }
  exit(0);
}
