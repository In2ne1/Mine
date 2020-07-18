//命名管道mkfifo应用(写端)

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>

int main()
{
  umask(0);
  int ret = mkfifo("./test.fifo", 0664);
  if(ret < 0 && errno != EEXIST)  //errno为EEXIST时，表示文件已经存在而报错
  {
    perror("mkfifo error");
    return -1;
  }
  int fd = open("./test.fifo", O_WRONLY);  //打开管道文件缓冲区
  if(fd < 0)
  {
    perror("open fifo error");
    return -1;
  }
  printf("open fifo success\n");
  int i = 0;
  while(1)
  {
    char buf[1024] = {0};
    //springf把格式化数据写入到第一个参数文件中，printf则是写入到标准输出中
    sprintf(buf, "i would like to buy a hamburger + %d", i++);
    write(fd, buf, strlen(buf));  //写入管道文件缓冲区
    printf("write data success\n");
    sleep(1);  //每隔一秒写一次
  }
  close(fd);
  return 0;
}
