#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
  FILE* fp = NULL;

  //FILE* fopen(const char* filename, const char* mode);  mode:r/r+/w/w+/a/a+
  fp = fopen("./test.txt", "r+");
  if(fp == NULL)
  {
    perror("fopen error");
    return -1;
  }

  //fseek()跳转读写位置,不定参 从文件 SEEK_SET起始/SEEK_CUR当前读写位置/SEEK_END末尾 偏移
  fseek(fp, 0, SEEK_SET);  //0表示偏移字符个数，可正可负

  char buf[] = "may the odds be ever in your fever\n";
  //size_t fwrite(const void* ptr,  size_t size, size_t nmemb, FILE* stream)
  int ret = fwrite(buf, 1, strlen(buf), fp);  //参数：数据、块大小、块个数、流指针
  if(ret == 0)
  {
    perror("fwrite error");
    return -1;
  }
  return 0;
}
