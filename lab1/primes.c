// 这个好难。确实不会写，看了别人的思路要用递归，然后看着一堆管道没明白怎么去传递管道信息。
// 之后查看了别人的实现，才看懂原理图那个除以当前元素余数不为0才传送的意义
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 从左侧管道获取第一个值，若存在，则输出
int lpipe(int fd[2],int *first)
{
  if (read(fd[0],first,sizeof(int))==sizeof(int))
  {
    printf("prime %d\n",*first);
    return 0;
  }
  return -1;
}

// 遍历去读取管道内容，若除以first余数不为0，传送到下一个管道中
void trans(int lpipe[2],int rpipe[2],int first)
{
  int data;
  while (read(lpipe[0],&data,sizeof(int))== sizeof(int))
  {
    if (data % first)
    {
      write(rpipe[1],&data,sizeof(int));

    }
  }
  close(lpipe[0]);
  close(rpipe[1]);

}

// 父进程回收子进程，子进程递归调用primes
void primes(int fd[2])
{
  close(fd[1]);
  int first;
  if (lpipe(fd,&first)==0)
  {       
    int p[2]; 
    pipe(p);
    trans(fd,p,first);
    if (fork()==0)
    { 
      primes(p);
    }
    else
    {
      close(p[0]);
      wait(0);
    }
  } 
  exit(0); 
}   

int main(int argc,char *argv[])
{  
  int fd[2];
  pipe(fd);
  for (int i=2;i<35;i++)
  {
    write(fd[1],&i,sizeof(int));
  }
  if (fork()==0)
  {
    primes(fd);
  }
  else
  {
    close(fd[0]);
    close(fd[1]);
    wait(0);
  }
  exit(0);

}
