#include<thread>
#include"cloudbackup.hpp"

void compress_test(char* argv[])
{
  //argv[1]-源文件名称  argv[2]-压缩包名称
  lyy::CompressTool::Compress(argv[1], argv[2]);
  std::string file = argv[2];
  file += ".txt";
  lyy::CompressTool::Uncompress(argv[2], file.c_str());
}

void datamanager_test()
{
  lyy::Datamanager data_manage("./test.txt");
  
  //data_manage.Insert("a.txt", ".txt");
  //data_manage.Insert("b.txt", "b.txt.gz");
  //data_manage.Insert("c.txt", "c.txt.gz");
  //data_manage.Insert("d.txt", "d.txt");
  
  data_manage.Load();
  data_manage.Insert("d.txt", "d.txt.gz");
  std::vector<std::string> list;
  data_manage.AllList(&list);  //获取所有信息
  for(auto i : list)
  {
    std::cout << i.c_str() << std::endl;
  }
  std::cout << "---------------------------------" << std::endl;
  list.clear();
  data_manage.NonCompressList(&list);  //获取未压缩文件列表
  for(auto i : list)
  {
    std::cout << i.c_str() << std::endl;
  }
}

void thr_noncompress()  //线程入口函数 非热点压缩模块
{
  lyy::NonHotCompress ncom(BACKUP_PATH, GZFILE_PATH);
  ncom.Start();
  return;
}
void nonhotcompress_test()
{
  bool ret = boost::filesystem::exists(BACKUP_PATH);  //文件备份路径不存在则创建
  if(!ret)
  {
    boost::filesystem::create_directory(BACKUP_PATH);
  }

  ret = boost::filesystem::exists(GZFILE_PATH);  //压缩存放路径不存在则创建
  if(!ret)
  {
    boost::filesystem::create_directory(GZFILE_PATH);
  }
  lyy::data_manage.Insert("aaa.txt", "aaa.txt");
  std::thread thr(thr_noncompress);  //c++11中的线程
  thr.join();  //等待线程退出
}

void thr_httpserver()  //线程入口函数 网络通信启动模块
{
  lyy::Server srv;
  srv.Start();
  return ;
}

int main(int argc, char* argv[])
{
  bool ret = boost::filesystem::exists(BACKUP_PATH);  //文件备份路径不存在则创建
  if(!ret)
  {
    boost::filesystem::create_directory(BACKUP_PATH);
  }

  ret = boost::filesystem::exists(GZFILE_PATH);  //压缩存放路径不存在则创建
  if(!ret)
  {
    boost::filesystem::create_directory(GZFILE_PATH);
  }
  std::thread thr_compress(thr_noncompress);  //压缩线程 启动非热点压缩模块
  std::thread thr_server(thr_httpserver);  //通信线程 网络通信服务端启动

  thr_compress.join();  //线程等待 
  thr_server.join();

  return 0;
}
