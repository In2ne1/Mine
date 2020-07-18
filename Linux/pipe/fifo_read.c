//命名管道mkfifo应用

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include <sys/stat.h>
#include<string.h>

int main()
{
  umask(0);
  int ret = mkfifo("./test.fifo", 0664);
  if(ret < 0 && errno != EEXIST)  //errno为EEXIST时，表示文件已经存在而报错
  {
    perror("mkfifo error");
    return -1;
  }
  int fd = open("./test.fifo", O_RDONLY);  //打开管道文件缓冲区
  if(fd < 0)
  {
    perror("open fifo error");
    return -1;
  }
  printf("open fifo success\n");
  while(1)
  {
    char buf[1024] = {0};
    int ret = read(fd, buf, 1023);  //读取管道文件缓冲区内容
    if(ret < 0)
    {
      perror("read error");
      return -1;
    }
    else if(ret == 0)  //没有读到内容
    {
      perror("all write closed");
      return -1;
    }
    printf("read buf : [%s]\n", buf);
  }
  close(fd);
  return 0;
}
