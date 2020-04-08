#include"cloudbackup.h"

#define DATA_FILE "./list.backup"  //数据管理模块的持久化存储文件
#define LISTEN_DIR "./backup/"  //监听目录
#define SERVER_IP "192.168.244.131"
#define SERVER_PORT 9000

int main()
{
	Client client(LISTEN_DIR, DATA_FILE, SERVER_IP, SERVER_PORT);
	client.Start();
	return 0;
}
