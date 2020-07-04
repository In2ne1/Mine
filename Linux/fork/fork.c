#include<stdio.h>  //printf头文件
#include<unistd.h>  //fork头文件
int main()
{
  printf("---create child process start---%d\n", getpid());
  pid_t pid;
  pid = fork();  //创建子进程,并接收fork返回值
  if(pid == 0)  //这是子进程
  {
    printf("this is child process! %d , rpid:%d\n", getpid(), pid);  //getpid函数：获取当前进程的pid
  }
  else if(pid > 0)  //这是父进程
  {
    printf("this is parents process! %d, rpid:%d\n",getpid(), pid);
  }
  else 
  {
    printf("create child process fail!\n");
  }
  printf("---create child process end---\n");
  return 0;
}
        
