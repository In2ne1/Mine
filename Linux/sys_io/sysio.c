#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
  int fd = 1;
  umask(0);  //将当前进程的文件创建权限掩码设置为0，仅在当前进程中有效
  //int open(const char* pathname, int flags, mode_t mode);
  //fd = open("./test.txt", O_RDWR | O_CREAT | O_EXCL, 0777);
  fd = open("./test.txt", O_RDWR | O_EXCL, 0777);  //防止和write冲突去掉O_CREAT
  if(fd < 0)
  {
    perror("open error\n");
    return -1;
  }

  lseek(fd, 10, SEEK_END);  //跳转到文件末尾
  char ptr[1024] = "may the odds be ever in your fever\n";
  //ssize_t write(int fd, const void* buf, size_t count);
  int ret = write(fd, ptr, strlen(ptr));
  if(ret < 0)
  {
    perror("write error\n");
    return -1;
  }
  printf("ret: %d \n", ret);

  lseek(fd, 0, SEEK_SET);  //跳转到文件起始位置
  char buf[1024] = {0};
  ret = read(fd, buf, 1024);
  if(ret < 0)
  {
    perror("read error");
    return -1;
  }
  printf("ret: %d - %s \n", ret, buf);
  close(fd);
  return 0;
}
