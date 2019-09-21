#pragma once
#include<iostream>
using namespace std;
#include<string.h>
#include<stdlib.h>
#define DEFAULTCAPA 16  //容量基数
#define COUNTCAPA(size) (((size)/DEFAULTCAPA + 1) * DEFAULTCAPA)  //扩容

//insert插入函数模拟实现，一种插入字符串的，一种在pos位置插入字符串str的前n个字符
//erase删除的重载，一种删除pos位len长度的字符，一种删除一个字符
//append连接函数模拟实现，一种n个的，一种直接续字符串的
//size、empty、push_Back、reserve、resize
//reverse翻转字符串中的单词
//find找字符函数的重载,一种找从pos位开始的一个字符，一种找从pos位开始的一个字符串，一种从pos位开始找String的字符容器
//重载[]、重载=（两种字符串赋值方式）、重载输出流os

class String
{
private:
	char* m_data;
	size_t m_size;
	size_t m_capacity;
public:
	String(const char* s = "") :  //构造,字符串初始化
		m_capacity(DEFAULTCAPA)
	{
		if (s == nullptr)
		{
			s = "";
		}
		m_size = strlen(s);
		m_capacity = COUNTCAPA(m_size);
		m_data = new char[m_capacity];
		strncpy(m_data, s, m_size);
	}

	String(String &s)  //拷贝构造，字符容器
	{
		m_size = s.m_size;
		m_capacity = s.m_capacity;
		m_data = new char[m_capacity];
		strncpy(m_data, s.m_data, s.m_size);
	}

	String(size_t n, char ch) :  //构造，n个字符串ch初始化
		m_size(n),
		m_capacity(COUNTCAPA(n))
	{
		m_data = new char[m_capacity];
		memset(m_data, ch, m_size);
	}

	~String()  //析构
	{
		if (m_data != nullptr)
		{
			delete[]m_data;
			m_data = nullptr;
		}
		m_size = m_capacity = 0;
	}

	String& insert(const char* str, int pos)  //插入字符串 or 改变传参为String，插入字符容器
	{
		size_t i, j;
		size_t n = strlen(str);
		size_t size = m_size;
		m_size += n;
		reserve(m_size);  //扩容
		for (i = size - 1; i > pos; i--)  // \0也要移
		{
			m_data[i + n] = m_data[i];  //pos位置之后的字符后移
		}
		++i;
		for (j = 0; j < n; j++, i++)  //插入字符
		{
			m_data[i] = str[j];
		}
		return *this;
	}

	String& insert(const char* str, int n, int pos)  //在pos位置插入字符串str的前n个字符
	{
		size_t i, j;
		size_t size = m_size;
		m_size += n;
		reserve(m_size);  //扩容
		for (i = size - 1; i > pos; i--)  // \0也要移
		{
			m_data[i + n] = m_data[i];  //pos位置之后的字符后移
		}
		++i;
		for (j = 0; j < n; j++, i++)  //插入字符
		{
			m_data[i] = str[j];
		}
		return *this;
	}

	String& erase(size_t len, size_t pos)  //删除pos位len长度的字符
	{
		if (pos + len >= m_size - 1)  //删除的长度大于字符的长度
		{
			m_data[pos] = '\0';
			m_size = pos;
		}
		else
		{
			strcpy(m_data + pos, m_data + pos + len);
			m_size -= len;
		}
		return *this;
	}

	String& erase(size_t pos)  //删除一个字符
	{
		if (pos >= m_size - 1)  //pos位置大于字符串长度
		{
			return *this;
		}
		else
		{
			strcpy(m_data + pos, m_data + pos + 1);
			m_size--;
		}
		return *this;
	}

	String& append(size_t n, char ch)  //模拟实现append，n个ch
	{
		for (size_t i = 0; i < n; i++)
		{
			push_Back(ch);
		}
		return *this;
	}

	String& append(const char *str)  //模拟实现append，一个字符串
	{
		for (size_t i = 0; i < strlen(str); i++)
		{
			push_Back(str[i]);
		}
		return *this;
	}

	size_t size()
	{
		return m_size;
	}

	bool empty()  //模拟实现判空
	{
		return m_size == 0;
	}

	void push_Back(char ch)  //尾插一个字符ch
	{
		COUNTCAPA(m_size);
		m_data[m_size] = ch;
		m_size++;
	}

	void reserve(size_t size)  //扩容
	{
		if (size >= m_capacity)
		{
			m_capacity = COUNTCAPA(size);
			m_data = (char*)realloc(m_data, m_capacity);
		}
	}

	void resize(size_t size, char ch = '\0')
	{
		reserve(size);

		if (m_size < size)
		{
			memset(m_data + m_size, ch, size - m_size);
		}
		m_size = size;
	}

	String& reverse(String& s)  //单词翻转
	{
		size_t i;
		char tmp;
		size_t start = 0, end = 0;
		for (i = 0; i < s.m_size; i++)
		{
			if (s.m_data[i] == ' ')  //找空格
			{
				end = i - 1;
				for (; start < end; start++, end--)   //翻转start到end之间的字符单词
				{
					tmp = m_data[start];
					m_data[start] = m_data[end];
					m_data[end] = tmp;
				}
				start = i + 1;
			}
		}
		end = i - 1;
		for (; start < end; start++, end--)  //翻转最后一个单词
		{
			tmp = m_data[start];
			m_data[start] = m_data[end];
			m_data[end] = tmp;
		}
		return s;
	}

	size_t find(char ch, int pos = 0)  //找字符,从pos位开始的一个字符
	{
		if (pos < 0 || pos >= m_size)  //位置越界
		{
			return -1;
		}
		char* tmp = strchr(m_data + pos, ch);
		if (tmp)
		{
			return tmp - m_data;
		}
		return -1;
	}
	
	size_t find(const char *str, int pos = 0)  //找字符串，从pos位开始的一个字符串
	{
		if (pos < 0 || pos >= m_size)
		{
			return -1;
		}
	
		char * tmp = strstr(m_data + pos, str);
	
		if (tmp)
		{
			return tmp - m_data;
		}
		return -1;
	}
	
	size_t find(const String &s, int pos = 0)  //找字符容器，从pos位开始找String的字符容器
	{
		if (pos < 0 || pos >= m_size)
		{
			return -1;
		}
	
		char * tmp = strstr(m_data + pos, s.m_data);
	
		if (tmp)
		{
			return tmp - m_data;
		}
		return -1;
	}

	char& operator[] (int i)  //中括号重载
	{
		return m_data[i];
	}

	String& operator = (const char* str)  //赋值，传字符串
	{
		m_size = strlen(str);
		reserve(m_size);  //扩容
		strcpy(m_data, str);
		return *this;
	}

	String& operator = (String &s)  //赋值运算符重载，传字符容器
	{
		m_size = s.m_size;
		reserve(m_size);
		strcpy(m_data, s.m_data);
		return *this;
	}
  
  

	friend ostream& operator << (ostream& os, String& s);  //输出流重载
};
ostream& operator << (ostream& os, String& s)
{
	size_t i;
	for (i = 0; i < s.m_size; i++)
	{
		os << s.m_data[i];
	}
	return os;
}
