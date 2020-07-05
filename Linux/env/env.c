#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
//程序中运行参数可以通过main函数的参数传入
//argc表示参数个数，argv存储参数信息
int main(int argc, char* argv[], char* env[])
{
  for(int i = 0; i < argc; i++)  //打印argv中的参数
  {
    printf("argv[%d] = [%s]\n", i, argv[i]);
  }

  //访问环境变量法一：
  for(int i = 0; env[i] != NULL; i++)  //打印env中的环境变量
  {
    printf("env[%d] = [%s]\n", i, env[i]);
  }
  printf("\n\n\n");

  //访问环境变量法二：
  extern char** environ;  //声明一个外部变量，这个变量实际在其他地方已经定义了，此处直接声明使用
  for(int i = 0; environ[i] != NULL; i++)
  {
    printf("environ[%d] = [%s]\n", i, environ[i]);
  }

  //只访问一个环境变量的内容
  char* ptr = getenv("MYENV");  //getenv搜索环境变量列表，通过环境变量名，返回该环境变量的value
  if(ptr != NULL)
    printf("MYENV = [%s]\n", ptr);
  else 
    printf("not find this variable\n");
  return 0;
}
