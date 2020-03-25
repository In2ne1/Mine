#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>  //套接字接口
using namespace std;

int main(int argc, char* argv[])
{
  //参数传入服务端要绑定的地址信息(IP地址和端口)
  if(argc != 3)
  {
    cout << "./srv 192.168.244.131 9000" << endl;
    return -1;
  }
  //通过参数获取地址信息(IP地址和端口)
  const char* ip_addr = argv[1];
  uint16_t port_addr = atoi(argv[2]);//atoi将字符串转换成整数值
  
  //1.创建套接字
  //int socket(int domain, int type, int pritocol);
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  //返回一个文件描述符作为操作句柄
  if(sockfd < 0)  //套接字接口创建失败
  {
    perror("socket error");
    return -1;
  }

  //2.为套接字绑定地址信息
  struct sockaddr_in addr;  //定义一个IPV4地址结构(有三个成员)
  addr.sin_family = AF_INET;  //给地址结构的成员赋值
  addr.sin_port = htons(port_addr);  //将主机字节序转换为网络字节序端口
  //int inet_pton(int af, const char* src, void dst);
  inet_pton(AF_INET, ip_addr, &addr.sin_addr.s_addr);  //将字符串IP地址转换为网络IP地址
  int ret;
  socklen_t len = sizeof(struct sockaddr_in);  //地址信息长度
  //int bind(int sockfd, struct sockaddr* addr, socklen_t len);
  ret = bind(sockfd, (struct sockaddr*)&addr, len);
  if(ret < 0)
  {
    perror("bind error");
    return -1;
  }

  //3.接收/发送数据
  while(1)
  {
    char buf[1024] = {0};  //存放对端发送的数据
    struct sockaddr_in cliaddr;  //存放对端地址信息
    len = sizeof(struct sockaddr_in);  //地址信息长度
    //recvfrom(int sockfd, char* buf, int buf_len, int flag, struct sockaddr* peer_addr, socklen_t* addr_len);
    ret = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&cliaddr, &len);
    if(ret < 0)  //接收数据失败
    {
      perror("recvfrom error");
      close(sockfd);  //关闭套接字
      return -1;
    }

     char cli_ip[32] = {0};  //接收ip转换后的缓冲区
     //uint16_t ntohs(uint16_t netshort);
     uint16_t cli_port = ntohs(cliaddr.sin_port);  //将网络字节序转换为主机字节序
     //const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);
     inet_ntop(AF_INET, &cliaddr.sin_addr, cli_ip, 32);
     cout << "client[" << cli_ip <<":" << cli_port << "] say:" << buf << endl;

     cout << "server say: ";
     memset(buf, 0x00, 1024);  //初始化buf缓冲区为0
     cin >> buf;  //标准输入获取数据放到buf中，然后响应给客户端

     //ssize_t sendto(int sockfd, char* data, int data_len, int flag, struct sockaddr*dest_addr, socklen_t addr_len);
     ret = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, len);
     if(ret < 0)
     {
       perror("sendto error");
       close(sockfd);
       return -1;
     }
  };

  //5.关闭套接字
  close(sockfd);
  return 0;
}

