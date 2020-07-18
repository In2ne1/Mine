//匿名管道
//匿名管道只能用于具有亲缘关系的进程间通信，通过子进程复制父进程的方式获取管道的操作句柄
//注：创建管道必须在创建子进程之前，否则子进程拿不到管道的操作句柄
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
  int pipefd[2] = {-1};  //pipefd[0]用于从管道读取数据、pipefd[1]用于向管道写入数据
  int ret = pipe(pipefd);
  if(ret < 0)
  {
    perror("pope error");
    return -1;
  }
  pid_t pid = fork();  //创建子进程
  if(pid == 0)  //子进程，从管道读数据
  {
    /*
    close(pipefd[1]);  //关闭父进程的写端，测试所有写端被关闭后继续读取数据时，会read完后不再阻塞并返回0
    sleep(3);
    */

    /*
    close(pipefd[0]);  //关闭子进程的读端,测试所有读端被关闭继续写入数据时会触发异常的特性
    sleep(1000);  //子进程不读取数据，先让父进程写入
    */
    char buf[1024] = {0};
    int rlen = read(pipefd[0], buf, 1023);  //read(fd, buf, count);
    printf("child:%d - %s", rlen, buf);
  }
  else if(pid > 0)  //父进程，向管道写数据
  {
    /*
    close(pipefd[1]);  //关闭父进程的写端，测试所有写端被关闭后继续读取数据时，会read完后不再阻塞并返回0
    sleep(1000);  //父进程不写入数据，先让子进程读取
    */

    /*
    close(pipefd[0]);  //关闭父进程的读端,测试所有读端被关闭继续写入数据时会触发异常的特性
    sleep(3);
    */
    sleep(3);  //让子进程先运行,测试管道内没有数据时进行读操作的情况 
    char* ptr = "i would like to buy a hamburger\n";
    int total_len = 0;
    while(1)  //循环向管道缓冲区写入数据
    {
      int wlen = write(pipefd[1], ptr, strlen(ptr));  //write(fd, buf, count);
      total_len += wlen;
      printf("total_len = %d\n", total_len);  //写满数据后展示管道缓冲区大小，大概4k
    }
  }
  return 0;
}
