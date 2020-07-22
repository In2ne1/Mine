#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void sigcb(int signo)  //ctrl + c 程序会执行这个函数
{
  printf("recv a signal no: %d\n", signo);
}

int main()
{
  //sighandler_t signal(int signum, sighandler_t handler);
  //signal(SIGINT, SIG_IGN);  //ctrl + c程序不会退出

  //修改信号处理方式：
  signal(SIGINT, sigcb);  //修改内核中SIGINT信号的回调函数指针
  while(1)
  {
    printf("ohlala~~\n");
    sleep(10);
  }
  return 0;
}
