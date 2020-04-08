#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<boost/filesystem.hpp>
#include<boost/algorithm/string.hpp>  //split头文件
#include"httplib.h"

class FileTool  //文件工具类
{
private:
public:
	static bool Read(const std::string &filename, std::string *body)  //读取文件内容
	{
		std::ifstream fin(filename, std::ios::binary);  //输入文件流
		if (!fin.is_open())  //打开失败
		{
			std::cout << "FileTool::Read : open file " << filename << " failed!" << std::endl;
			return false;
		}
		int64_t fsize = boost::filesystem::file_size(filename);  //获取文件大小
		body->resize(fsize);  //给body申请空间接收文件数据
		fin.read(&(*body)[0], fsize);  //body是指针需要先解引用
		if (!fin.good())  //判断上一次操作(read)是否正常
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
		if (!fout.is_open())  //打开失败
		{
			std::cout << "FileTool::Write : open file " << filename << " failed!" << std::endl;
			return false;
		}
		fout.write(&body[0], body.size());
		if (!fout.good())
		{
			std::cout << "FileTool::Write : file " << filename << " write data failed!" << std::endl;
			return false;
		}
		fout.close();
		return true;
	}
};

class DataManager  //客户端文件数据管理模块
{
private:
	std::string m_backfile;  //持久化存储文件
	std::unordered_map<std::string, std::string> m_filelist;  //文件数据管理容器
public:
	DataManager(const std::string &filename):
		m_backfile(filename)
	{}
	bool Insert(const std::string &key, const std::string &val)  //插入/更新数据
	{
		m_filelist[key] = val;
		Storage();  //插入新文件，更新持久化存储
		return true;
	}
	bool GetEtag(const std::string &key, std::string *val)  //根据文件名获取文件etag信息
	{
		auto it = m_filelist.find(key);
		if (it == m_filelist.end())
		{
			return false;
		}
		*val = it->second;
		return true;
	}
	bool Storage()  //持久化存储
	{
		//存储方式：filename etag\r\n
		std::stringstream tmp;  //创建一个string流对象
		for (auto it = m_filelist.begin(); it != m_filelist.end(); it++)
		{
			tmp << it->first << " " << it->second << "\r\n";
		}
		FileTool::Write(m_backfile, tmp.str());  //持久化存储写入
		return true;
	}
	bool Load()  //初始化加载原有数据
	{
		//从数据的持久化存储文件中加载数据
	    //存储方式：filename gzfilename\r\nfilename gzfilename\r\n
	    //先将持久化存储备份文件的数据读取出来
		std::string body;
		if (!FileTool::Read(m_backfile, &body))
		{
			return false;
		}
		std::vector<std::string> list;
		//进行字符串处理，按照\r\n进行分割
		boost::split(list, body, boost::is_any_of("\r\n"), boost::token_compress_off);
		//每一行按照空格进行分割，前面时key，后面是val
		for (auto i : list)
		{
			size_t pos = i.find(" ");
			if (pos == -1)
			{
				continue;
			}
			std::string key = i.substr(0, pos);
			std::string val = i.substr(pos + 1);
			//将key/val添加到m_filelist中
			Insert(key, val);
		}
		return true;
	}
};

class Client  //云备份客户端模块
{
private:
	std::string m_srv_ip;  //服务端ip地址
	uint16_t m_srv_port;  //服务端端口
	std::string m_listen_dir;  //监控的目录名称
	DataManager data_manage;  //文件数据管理对象
public:
	Client(const std::string &filename, const std::string &data_file, 
			const std::string srv_ip, uint16_t srv_port) :
		m_listen_dir(filename),
		data_manage(data_file),
		m_srv_ip(srv_ip),
		m_srv_port(srv_port)
	{}
	bool NonBackUpFileList(std::vector<std::string> *list)  //获取需要备份的文件列表
	{
		if (!boost::filesystem::exists(m_listen_dir))
		{
			boost::filesystem::create_directory(m_listen_dir);  //目录不存在则创建
		}
		//进行目录监控，获取指定目录下所有文件名称
		boost::filesystem::directory_iterator begin(m_listen_dir);
		boost::filesystem::directory_iterator end;
		//逐个文件计算自身当前etag
		for (; begin != end; begin++)
		{
			if (boost::filesystem::is_directory(begin->status()))  //这个目录下的文件名是目录
			{
				continue;  //不做多层级目录备份，遇到直接跳过
			}
			std::string file_path = begin->path().string();  //带路径文件名
			std::string file_name = begin->path().filename().string();  //文件名
			//当前etag与data_manage中保存的原etag进行比较，看是否需要更新备份文件
			std::string cur_etag, old_etag;
			GetEtag(file_path, &cur_etag);  //新etag
			data_manage.GetEtag(file_name, &old_etag);  //旧etag
			if (cur_etag != old_etag)
			{
				list->push_back(file_name);  //新旧etag不同，文件需要重新备份
			}
		}
		return true;
	}
	bool GetEtag(const std::string &filepath, std::string *etag)  //计算文件etag
	{
		//etag内包含文件大小和文件最后一次修改时间
		int64_t fsize = boost::filesystem::file_size(filepath);
		time_t mtime = boost::filesystem::last_write_time(filepath);
		*etag = std::to_string(fsize) + "-" + std::to_string(mtime);
		return true;
	}
	bool Start()  //文件备份流程接口
	{
		data_manage.Load();
		while (1)  //循环备份
		{
			std::vector<std::string> list;
			NonBackUpFileList(&list);  //获取需要备份的文件列表
			for (int i = 0; i < list.size(); i++)
			{
				std::string file_name = list[i];
				std::string file_path = m_listen_dir + file_name;

				std::cout << file_path << " is need to backup!" << std::endl;

				std::string body;  //文件内容作为请求正文
				FileTool::Read(file_path, &body);
				httplib::Client client(m_srv_ip, m_srv_port);  //实例化一个Client对象并传入服务端端口信息
				std::string  req_path = "/" + file_name;
				auto rsp = client.Put(req_path.c_str(), body, "application/octet-stream");
				if (rsp == NULL || rsp->status != 200)
				{
					std::cout << file_path << " backup failed!" << std::endl;
					continue;  //该文件上传备份失败
				}
				std::string etag;
				GetEtag(file_path, &etag);
				data_manage.Insert(file_name, etag);  //备份成功，更新文件信息

				std::cout << file_path << " backup success!" << std::endl;
			}
			Sleep(1000);  //休眠一秒钟
		}
		return true;
	}
};

