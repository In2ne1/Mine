#include<iostream>
#include<string>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
using namespace std;

class UdpSocket
{
private:
  int m_sockfd;
public:
  //1.创建套接字
  bool Socket()
  {
    m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(m_sockfd < 0)
    {
      perror("socket error");
      return false;
    }
    return true;
  }
  void Addr(struct sockaddr_in* addr, const string &ip, const uint16_t port)  //地址信息结构体赋值
  {
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr->sin_addr.s_addr);
  }

  //2.绑定地址信息
  bool Bind(const string &ip, const uint16_t port)
  {
    struct sockaddr_in addr;
    Addr(&addr, ip, port);
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = bind(m_sockfd, (struct sockaddr*)&addr, len);
    if(ret < 0)
    {
      perror("bind error");
      return false;
    }
    return true;
  }

  //3.接收数据
  bool Recv(string* buf, string* dst_ip = NULL, uint16_t* dst_port = NULL)
  {
    char tmp[4096] = {0};
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = recvfrom(m_sockfd, tmp, 4096, 0, (struct sockaddr*)&addr, &len);
    if(ret < 0)
    {
      perror("recvfrom error");
      return false;
    }
    buf->assign(tmp, ret);  //给buf开辟ret长度的空间，然后从tmp中拷贝ret长度的数据过来
    if(dst_ip != NULL)
      *dst_ip = inet_ntoa(addr.sin_addr);
    if(dst_port != NULL)
      *dst_port = ntohs(addr.sin_port);
    return true;
  }

  //4.发送数据
  bool Send(const string& data, const string& ip, const uint16_t port)
  {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    Addr(&addr, ip, port);
    int ret = sendto(m_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
    if(ret < 0)
    {
      perror("sendto error");
      return false;
    }
    return true;
  }

  //5.关闭套接字
  bool Close()
  {
    close(m_sockfd);
    return true;
  }
};

#define CHECK_RET(q) if((q) == false){return -1;}
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    cout << "./cli 192.168.244.131 9000" << endl;  //服务端地址
    return -1;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  UdpSocket client;
  CHECK_RET(client.Socket());
  while(1)
  {
    cout << "client say:";
    string buf;
    cin >> buf;
    client.Send(buf, ip, port);

    buf.clear();
    client.Recv(&buf);
    cout << "server say: " << buf.c_str() << endl;
  }
  client.Close();

  return 0;
}
