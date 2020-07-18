//命令行实现管道通信：ps -ef | grep ssh

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>

int main()
{
  int pipefd[2] = {-1};
  int ret = pipe(pipefd);
  if(ret < 0)
  {
    perror("pipe error");
    return -1;
  }
  pid_t ps_pid = fork();
  if(ps_pid == 0)  //子进程ps
  {
    dup2(pipefd[1], 1);  //将标准输出重定向到管道写入端，向1写入数据相当于向管道写入数据
    execlp("ps", "ps", "-ef", NULL);  //程序替换 int execlp(const char *file, const char *arg, ...);
    exit(0);  //所有进程退出，匿名管道也会被释放
  }
  pid_t grep_pid = fork();
  if(grep_pid == 0)  //子进程grep
  {
    close(pipefd[1]);  //关闭写端，ps进程一旦退出即所有写端被关闭，grep读完数据后返回0
    //int dup2(int oldfd, int newfd);
    dup2(pipefd[0], 0);  //将标准输入重定向到管道读取端，从0读取数据就相当于从管道读取数据
    execlp("grep", "grep", "ssh", NULL);
    exit(0);
  }
  close(pipefd[0]);
  close(pipefd[1]);
  //进程等待
  waitpid(ps_pid, NULL, 0);  //pid_t waitpid(pid_t pid, int *status, int options);
  waitpid(grep_pid, NULL, 0);
  return 0;
}
