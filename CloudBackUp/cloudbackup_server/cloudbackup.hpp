#include<cstdio>
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<unordered_map>
#include<zlib.h>
#include<pthread.h>
#include<boost/filesystem.hpp>
#include<boost/algorithm/string.hpp>
#include"httplib.h"

#define NONHOT_TIME 10  //最后一次访问时间在10秒以外
#define INTERVAL_TIME 30  //非热点的检测30秒一次
#define BACKUP_PATH "./backup/"  //要备份的文件路径
#define GZFILE_PATH "./gzfile/"  //压缩后的文件路径
#define DATA_FILE "./list.backup"  //数据管理模块的数据备份文件名称

namespace lyy
{
  class FileTool
  {
    private:
    public:
      static bool Read(const std::string &filename, std::string *body)  //读取文件内容
      {
        std::ifstream fin(filename, std::ios::binary);  //输入文件流
        if(!fin.is_open())  //打开失败
        {
          std::cout << "FileTool::Read : open file " << filename << " failed!" << std::endl;
          return false;
        }
        int64_t fsize = boost::filesystem::file_size(filename);  //获取文件大小
        body->resize(fsize);  //给body申请空间接收文件数据
        fin.read(&(*body)[0], fsize);  //body是指针需要先解引用
        if(!fin.good())  //判断上一次操作(read)是否正常
        {
          std::cout << "FileTool::Read : file " << filename << " read data failed!" << std::endl;
          return false;
        }
        fin.close();
        return true;
      }
      static bool Write(const std::string &filename, const std::string &body)  //向文件内写入数据
      {
        std::ofstream fout(filename, std::ios::binary);  //ofstrem默认打开文件时会清空原有内容
        if(!fout.is_open())  //打开失败
        {
          std::cout << "FileTool::Write : open file " << filename << " failed!" << std::endl;
          return false;
        }
        fout.write(&body[0], body.size());
        if(!fout.good())
        {
          std::cout << "FileTool::Write : file " << filename << " write data failed!" << std::endl;
          return false;
        }
        fout.close();
        return true;
      }
  };

  class CompressTool
  {
    public:
      static bool Compress(const std::string &src, const std::string &dst)  //压缩
      {
        std::string body;
        FileTool::Read(src, &body);
        gzFile gf = gzopen(dst.c_str(), "wb");  //打开压缩包
        if(gf == NULL)
        {
          std::cout << "CompressTool::Compress : open file " << dst << " failed!" << std::endl;
          return false;
        }
        int wlen = 0;
        while(wlen < body.size())  //循环防止body中的数据没有一次性压缩完
        {
          int ret = gzwrite(gf, &body[wlen], body.size() - wlen);
          if(ret == 0)
          {
            std::cout << "CompressTool::Compress : file " << dst << " write compress data failed!"
              << std::endl;
            return false;
          }
          wlen += ret;
        }
        gzclose(gf);
        return true;
      }
      static bool Uncompress(const std::string &src, const std::string &dst) 
      {  //解压缩--压缩包名称-源文件名称
        std::ofstream fout(dst, std::ios::binary);
        if(!fout.is_open())  //打开失败
        {
          std::cout << "CompressTool::Uncompress : open file " << dst << " failed!" << std::endl;
          return false;
        }
        gzFile gf = gzopen(src.c_str(), "rb");  //读取压缩包文件内容
        if(gf == NULL)
        {
          std::cout << "CompressTool::Uncompress : open file " << src << " failed!" << std::endl;
          fout.close();  //压缩文件打开失败但是源文件打开成功了，返回时需要关闭
          return false;
        }
        char tmp[4096] = {0};
        int ret = 4096;
        while(ret > 0)
        {
          ret = gzread(gf, tmp, 4096);
          fout.write(tmp, ret);
        }
        fout.close();
        gzclose(gf);
        return true;
      }
  };

  class Datamanager
  {
    private:
      std::string m_backfile;  //持久化存储文件名称
      std::unordered_map<std::string, std::string> m_filelist;  //数据管理容器
      pthread_rwlock_t m_rwlock;  //读写锁
    public:
      Datamanager(const std::string &path) :
        m_backfile(path)
      {
        pthread_rwlock_init(&m_rwlock, NULL);
      }
      ~Datamanager()
      {
        pthread_rwlock_destroy(&m_rwlock);
      }
      bool Exists(const std::string &filename)  //判断文件是否存在
      {
        pthread_rwlock_rdlock(&m_rwlock);
        auto it = m_filelist.find(filename);
        if(it == m_filelist.end())
        {
          pthread_rwlock_unlock(&m_rwlock);
          return false;
        }
        pthread_rwlock_unlock(&m_rwlock);
        return true;
      }
      bool IsCompressed(const std::string &filename)  //判断文件是否压缩
      {
        pthread_rwlock_rdlock(&m_rwlock);
        auto it = m_filelist.find(filename);
        if(it == m_filelist.end())  //该文件不存在
        {
          pthread_rwlock_unlock(&m_rwlock);
          return false;
        }
        if(it->first == it->second)  //数据：源文件名称-压缩文件名称
        {
          pthread_rwlock_unlock(&m_rwlock);
          return false;  //两个名称一样表示未压缩
        }
        pthread_rwlock_unlock(&m_rwlock);
        return true;
      }
      bool NonCompressList(std::vector<std::string> *list)  //未压缩文件名称列表
      {
        pthread_rwlock_rdlock(&m_rwlock);
        //遍历文件列表，将未压缩的文件名存入list中
        for(auto it = m_filelist.begin(); it != m_filelist.end(); it++)
        {
          if(it->first == it->second)
          {
            list->push_back(it->first);
          }
        }
        pthread_rwlock_unlock(&m_rwlock);
        return true;
      }
      bool Insert(const std::string &src, const std::string &dst)  //插入文件数据
      {
        pthread_rwlock_wrlock(&m_rwlock);
        m_filelist[src] = dst;
        pthread_rwlock_unlock(&m_rwlock);
        Storage();  //更新修改后重新备份
        return true;
      }
      bool AllList(std::vector<std::string> *list)  //获取所有文件名称
      {
        pthread_rwlock_rdlock(&m_rwlock);
        for(auto it = m_filelist.begin(); it != m_filelist.end(); it++)
        {
          list->push_back(it->first);  //将所有源文件名称放入list中
        }
        pthread_rwlock_unlock(&m_rwlock);
        return true;
      }
      bool GetGzName(const std::string &src, std::string *dst)  //根据文件名获取压缩包名称
      {
        auto it = m_filelist.find(src);
        if(it == m_filelist.end())  //没找到文件
          return false;
        *dst = it->second;
        return true;
      }
      bool Storage()  //当数据改变时持久化存储文件
      {
        std::stringstream tmp;  //创建一个string流对象
        pthread_rwlock_rdlock(&m_rwlock);
        for(auto it = m_filelist.begin(); it != m_filelist.end(); it++)
        {
          tmp << it->first << " " << it->second << "\r\n";
        }
        pthread_rwlock_unlock(&m_rwlock);  //解锁
        FileTool::Write(m_backfile, tmp.str());  //持久化存储写入时不需要在锁内进行
        return true;
      }
      bool Load()  //服务端启动时，加载所有文件信息到内存
      {
        //从数据的持久化存储文件中加载数据
        //存储方式：filename gzfilename\r\nfilename gzfilename\r\n
        //先将持久化存储备份文件的数据读取出来
        std::string body;
        if(!FileTool::Read(m_backfile, &body))
        {
          return false;
        }
        std::vector<std::string> list;
        //进行字符串处理，按照\r\n进行分割
        boost::split(list, body, boost::is_any_of("\r\n"), boost::token_compress_off);
        //每一行按照空格进行分割，前面时key，后面是val
        for(auto i : list)
        {
          size_t pos = i.find(" ");
          if(pos == -1)
          {
            continue;
          }
          std::string key = i.substr(0,pos);
          std::string val = i.substr(pos + 1);
          //将key/val添加到m_filelist中
          Insert(key, val);
        }
        return true;
      }
  };

  lyy::Datamanager data_manage(DATA_FILE);  //全局

  class NonHotCompress
  {
    private:
      std::string m_gz_path;  //压缩后文件存储路径
      std::string m_bu_path;  //压缩前文件存储路径
      bool FileIsHot(const std::string &filename)  //判断文件是否是热点文件
      {
        time_t cur_t = time(NULL);  //获取当前时间
        struct stat st;
        if(stat(filename.c_str(), &st) == -1)  //获取文件信息，成功返回0，失败返回-1
        {
          std::cout << "NonHotCompress::FileIsHot : get file " << filename << " stat failed!" 
            << std::endl;
          return false;
        }
        if((cur_t - st.st_atime) > NONHOT_TIME)
        {
          return false;  //非热点文件返回false
        }
        return true;  //NONHOT_TIME以内的都是热点文件
      }
    public:
      NonHotCompress(const std::string bu_path, const std::string gz_path) :
        m_bu_path(bu_path),
        m_gz_path(gz_path)
      {}
      bool Start()  //开始压缩模块接口
      {
        while(1)
        {
          std::vector<std::string> list;
          data_manage.NonCompressList(&list);  //获取未压缩文件列表
          for(int i = 0; i < list.size(); i++)
          {
            if(!FileIsHot(list[i]))  //逐个判断未压缩文件是否是非热点文件
            {
              std::cout << "There is a NonHotUncompress file " << list[i] << std::endl;
              std::string src_name = list[i];  //源文件名称
              std::string dst_name = list[i] + ".gz";
              std::string src_path = m_bu_path + src_name;  //源文件路径+名称
              std::string dst_path = m_gz_path + dst_name;
              if(CompressTool::Compress(src_path, dst_path))  //压缩要带路径
              {
                data_manage.Insert(src_name, dst_name);  //更新数据信息，不带路径
                unlink(src_path.c_str());  //删除源文件
              }
            }
          }
          sleep(INTERVAL_TIME);  //休眠一会儿
        }
        return true;
      }
  };
  
  class Server
  {
    private:
      //std::string m_file_path;  //文件上传备份路径
      httplib::Server m_server;

      static void UpLoad(const httplib::Request &req, httplib::Response &rsp)  //文件上传处理回调函数
      {
        std::string filename = req.matches[1];  //捕捉到的客户要上传的文件名
        std::string filepath = BACKUP_PATH + filename;  //文件备份路径
        FileTool::Write(filepath, req.body);
        data_manage.Insert(filename, filename);  //添加文件信息到数据管理模块
        rsp.status = 200;
        return ;
      }
      static void List(const httplib::Request &req, httplib::Response &rsp)  //文件列表处理
      {
        std::vector<std::string> list;
        data_manage.AllList(&list);
        std::stringstream tmp;
        tmp << "<html><body><hr />";  //html标签开始
        for(int i = 0; i < list.size(); i++)  //组织响应html网页数据
        {
          //网页显示为："<a href='download/a.txt'> a.txt </a>"
          tmp << "<a href='/download/" << list[i] << "'>" << list[i] << "</a>";  //可点击下载的超链接
          tmp << "<hr />";  //分割线
        }
        tmp << "<hr /></body></html>";  //html标签结尾

        rsp.set_content(tmp.str().c_str(), tmp.str().size(), "text/html");
        //rsp.set_content(正文数据, 正文数据大小, 正文类型);
        //相当于rsp.body = tmp.str().c_str();  rsp.set_header("Content-Type", "text/html");
        rsp.status = 200;
        return ;
      }
      static void DownLoad(const httplib::Request &req, httplib::Response &rsp)  //文件下载处理
      {
          //matches[0]是整体字符串，[1]是.*捕捉到的字符串
          std::string filename = req.matches[1];  //将匹配到的字符(客户输入下载的文件名)暂存到这里
          bool ret = data_manage.Exists(filename);
          if(!ret)  //文件不存在
          {
            rsp.status = 404;
            return ;
          }
          std::string file_path = BACKUP_PATH + filename; 
          ret = data_manage.IsCompressed(filename);
          if(ret)  //文件被压缩了
          {
            std::string gzname;
            data_manage.GetGzName(filename, &gzname);  //获取压缩包文件名称
            std::string gzfile_path = GZFILE_PATH + gzname;
            CompressTool::Uncompress(gzfile_path, file_path);  //解压缩
            unlink(gzfile_path.c_str());  //删除压缩包
            data_manage.Insert(filename, filename);  //更新文件状态
          }
          FileTool::Read(file_path, &rsp.body);  //直接将文件读取到rsp的body中
          rsp.set_header("Content-Type", "application/octet-stream");  //二进制流下载文件
          rsp.status = 200;
          return ;
      } 
    public:
      bool Start()  //启动网络通信模块接口
      {
        m_server.Put("/(.*)", UpLoad);  //针对PUT请求 调用Upload函数进行响应
        m_server.Get("/list", List);
        //.表示匹配除\r,\n外的任意单个字符，*表示匹配前面的子表达式任意次
        m_server.Get("/download/(.*)", DownLoad);  //.*正则表达式 

        m_server.listen("0.0.0.0", 9000);  //开始监听主机任意网卡

        return true;
      }
  };
}

