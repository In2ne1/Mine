#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main()
{
  //kill(getpid(), SIGHUP);  //给指定进程发送指定的信号 int kill(pid_t pid, int sig);
  //raise(SIGINT);  //给进程自己发送指定的信号，省略了getpid()步骤
  //abort();  //给进程自己发送SIGABRT信号(通常用于异常通知)
  //alarm(3);  //3s钟之后给进程自己发送SIGALRM信号

  while(1)
  {
    printf("ohlala~~\n");
    sleep(1);
  }
  return 0;
}

