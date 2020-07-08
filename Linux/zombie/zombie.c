#include<stdio.h>  //printf
#include<stdlib.h>  //exit
#include<unistd.h>  //sleep、fork
#include<sys/wait.h>

int main()
{
  pid_t pid = fork();
  if(pid == 0)
  {
    //chile process
    sleep(5);
    exit(66);  //父进程对于子进程的退出如果什么都不做，那么子进程就会成为僵尸进程
  }

  /*
   * wait()是父进程阻塞等待子进程退出 
   * waitpid()是非阻塞，若无子进程退出则直接报错返回，故需要循环判断
   * waitpid返回值>0表示获取到退出的子进程pid；==0表示现在还无子进程退出；<0表示无子进程则出错
  */

  //wait(NULL);  //父进程阻塞等待子进程退出，先暂时不考虑获取子进程退出的返回值
  int status;  //status保存子进程返回值
  while(waitpid(pid, &status, WNOHANG) == 0)
  {
    printf("现在子进程还未退出\n");
    sleep(1);
  }
  printf("status: [%d] \n", (status >> 8) & 0xff);  //输出子进程返回值status,只要低16位中的高8位
  
  while(1)
  {
    printf("父进程循环\n");
    sleep(1);
  }
  return 0;
}
