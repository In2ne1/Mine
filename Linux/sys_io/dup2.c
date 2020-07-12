#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
  /*
  umask(0);
  int fd = open("./test.txt", O_RDWR | O_CREAT, 0664);
  if(fd < 0)
  {
    perror("open error");
    return -1;
  }
  dup2(fd, 1);  //将1重定向到test.txt这个文件(0标准输入、1标准输出、2标准错误)
  printf("fd = %d\n", fd);
  fflush(stdout);  //刷新到标准输出缓冲区
  close(fd);
  */

  FILE*fp = fopen("./test.txt", "r+");
  fp->_fileno = 1;  //将test.txt文件流指针中的文件描述符改成标准输出的描述符
  fwrite("la~li~hou~~~\n", 1, 12, fp);
  fclose(fp);
  
  return 0;
}
