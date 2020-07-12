#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
  while(1)
  {
    printf("[test@192 minishell]$ ");
    fflush(stdout);  //刷新一下标准输出缓冲区
    
    //1.等待标准输入
    char buf[1024] = {0};
    //char* fgets(char* s, int size, FILE* stream);
    fgets(buf, 1023, stdin); 
    buf[strlen(buf) - 1] = '\0';  //将最后的\n换成\0
    
    //1.5解析重定向: ls -l > a.txt
    char* ptr = buf;
    int redirect_flag = 0;
    char* redirect_file = NULL;
    while(*ptr != '\0')
    {
      if(*ptr == '>')
      {
        redirect_flag = 1;  //清空重定向>
        *ptr = '\0';  //将>替换成结尾标志，后面解析指令时就解析到此处结束
        ptr++;
        if(*ptr == '>')  //追加重定向>>
        {
          redirect_flag = 2;
          ptr++;
        }
        while(*ptr == ' ' && *ptr != '\0')
          ptr++;  //将a.txt之前的空格走完
        redirect_file = ptr;
        while(*ptr != ' ' && *ptr != '\0')  //ptr走完文件名a.txt
          ptr++;
        //prt = '\0';
      }
      ptr++;
    }

    //2.对输入数据进行解析
    ptr = buf;
    char* argv[32] = {NULL};
    int argc = 0;
    while(*ptr != '\0')
    {
      if(*ptr != ' ')
      {
        argv[argc] = ptr;
        argc++;
        while(*ptr != ' ')  //将一个完整指令放入argv后指针后移到指令后的第一个空格位置
          ptr++;
        *ptr = '\0';  //解析一个指令后，将该指令后的一个空格置为\0
      }
      ptr++;
    }
    
    //3.创建子进程
    pid_t pid = fork();
    
    //4.在子进程中进行程序替换
    if(pid == 0)  //子进程
    {
      if(redirect_flag == 1)  //清空重定向
      {
        int fd = open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
        dup2(fd, 1);  //将标准输出重定向到redirect_file，原本要打印的数据就会被写入到文件
      }
      else if(redirect_flag == 2)  //追加重定向 
      {
        int fd = open(redirect_file, O_WRONLY|O_CREAT|O_APPEND, 0664);
        dup2(fd, 1);
      }
      //execvp(const char* file, char* const argv[]);
      execvp(argv[0], argv);  //argv[0]里面存放的就是要被替换的程序名
      //perror打印上一次系统调用接口使用的错误原因
      perror("execvp error");  //程序替换失败，接着向下执行，若成功则不再运行此程序
      exit(0);
    }
    
    //5.进程等待
    wait(NULL);  //不关心子进程返回值则参数传NULL
  }
  return 0;
}
