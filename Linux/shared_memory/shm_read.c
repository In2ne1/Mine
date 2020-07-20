//进程间通信-共享内存
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/shm.h>

#define IPC_KEY 0x12345678

int main()
{
  //1.创建共享内存
  //int shmget(key_t key, size_t size, int shmflg);  参数(标识符，大小，标志位|权限)
  int shm_id = shmget(IPC_KEY, 32, IPC_CREAT|0664);
  if(shm_id < 0)
  {
    perror("shmget error");
    return -1;
  }
  //2.建立映射关系
  //void *shmat(int shmid, const void *shmaddr, int shmflg);  参数(操作句柄，映射首地址，操作权限)
  void* shm_start = shmat(shm_id, NULL, 0);
  if(shm_start == (void*)-1)
  {
    perror("shmat error");
    return -1;
  }
  //3.进行内存操作
  while(1)
  {
    printf("[%s]\n", shm_start);  //shm_start是指向共享内存首地址的指针
    sleep(1);
  }
  //4.解除映射关系
  //int shmdt(const void *shmaddr);  参数(共享内存首地址)
  shmdt(shm_start);
  //5.删除共享内存
  //int shmctl(int shmid, int cmd, struct shmid_ds *buf);参数(操作句柄，要进行的操作IPC_RMID，共享内存信息结构)
  shmctl(shm_id, IPC_RMID, NULL);
  return 0;
}
