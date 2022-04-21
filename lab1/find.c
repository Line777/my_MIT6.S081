#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path,const char *filename)
{
  char buf[512],*p;
  int fd;
  struct dirent de;  //用于获取目录内容
  struct stat st;
  if ((fd=open(path,0))<0) //打不开目录
  {
    fprintf(2,"find: can't open %s\n",path);
    exit(0);
  }
  if (fstat(fd,&st) <0)   //获取文件状态，存放在st中
  {
    fprintf(2,"find: can't fstat%s\n",path);
    close(fd);
    exit(0);
  }
  if (st.type!= T_DIR)  //如果文件类型不是目录，则提示错误，退出
  {
    fprintf(2,"find: %s is not a directory\n",path);
    close(fd);
    exit(0);
  }
  if (strlen(path)+1+DIRSIZ+1>sizeof(buf))  //DIRSIZ:14，为stat结构体中文件名的长度
  {
    fprintf(2,"find: path is too long\n");
    exit(0);
  }
  strcpy(buf,path);  //将path复制到buf
  p=buf+strlen(buf); //将指针p移动到buf的末尾
  *p++='/';          //path后加'/'
  while (read(fd,&de,sizeof(de))==sizeof(de))  //从目录中循环获取目录内容
  {
    if (de.inum==0)
    {
      continue;
    }
    memmove(p,de.name,DIRSIZ);  //在字符串buf后加文件名
    p[DIRSIZ]=0;                //字符串结束标志
    if (stat(buf,&st)<0)        //获取文件状态
    {
      fprintf(2,"find: cannot stat %s\n",buf);
      continue;
    }
    if (st.type==T_DIR && strcmp(p,".")!=0 && strcmp(p,"..")!=0)
    {
      find(buf,filename);  //若是目录，递归查找文件
    }
        else if (strcmp(filename,p)==0)
    {
      printf("%s\n",buf);  //若文件名相同，则输出结果
    }
  }
  close(fd);
}

int main(int argc,char *argv[])
{

  if (argc<3)
  {
    printf("usage find directory filename\n");
    exit(0);
  }
  find(argv[1],argv[2]);  //查找目录名和文件名
  exit(0);
}
