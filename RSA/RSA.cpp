#include"RSA.h"
#include<iostream>
#include<time.h>
#include<fstream>
#include <boost/multiprecision/miller_rabin.hpp>
using namespace std;
using namespace boost;

#define NUMBER 256

RSA::RSA()  //构造函数
{
	getKeys();
}


DataType RSA::dealEncryption(DataType data, DataType ekey, DataType pkey)  //加密函数(e, n)
{
	/*
	return (DataType)pow(data, ekey) % pkey;  //幂运算会导致数据溢出
	a^b % c  //可用同余定理进行改进
	*/

	DataType Ai = data;  //每一项
	DataType msgE = 1;
	while (ekey)
	{
		if (ekey & 1)
			msgE = (Ai * msgE) % pkey;  //当前结果 = (前一项结果 * 当前项) % c
		ekey >>= 1;  //更新b
		Ai = (Ai * Ai) % pkey;  //更新Ai为前一项结果的乘积再取模，Ai = [A(i - 1)*A(i - 1)] % c
	}
	return msgE;
}
DataType RSA::dealDeciphering(DataType data, DataType dkey, DataType pkey)  //解密函数(d, n)
{
	return dealEncryption(data, dkey, pkey);
}


//接口
void RSA::Encryption(const char* filename, const char* fileout)  //加密函数接口
{
	ifstream fin(filename);  //创建输入文件流对象，并选择以二进制形式打开
	ofstream fout(fileout);
	if (!fin.is_open())  //判断文件是否打开
		return;

	char* buffer = new char[NUMBER];  //设置读取缓冲区字符
	DataType* bufferOut = new DataType[NUMBER];  //设置加密后文件内容的输出文件
	while (!fin.eof())  //读取文件，并进行加密
	{
		fin.read(buffer, NUMBER);  //读取要加密的文件内容，并放入缓冲区中
		int curNum = fin.gcount();  //最近一次读取到的字节数

		for (int i = 0; i < curNum; i++)  //加密，并将加密后的信息输出到文件中
		{
			bufferOut[i] = dealEncryption(buffer[i], m_key.ekey, m_key.pkey);
		}
		fout.write((char*)bufferOut, curNum * sizeof(DataType));
	}
	fin.close();
	fout.close();
}
void RSA::Deciphering(const char* filename, const char* fileout)  //解密函数接口
{
	ifstream fin(filename);
	ofstream fout(fileout);
	if (!fin.is_open())
		return;
	DataType* buffer = new DataType[NUMBER];  //设置读取加密文件缓冲区
	char* bufferOut = new char[NUMBER];   //设置加密后输出缓冲区
	while (!fin.eof())
	{
		fin.read((char*)buffer, NUMBER * sizeof(DataType));  //从buffer中读取加密内容
		int num = fin.gcount();  //获取最近一次读取的字节数
		num /= sizeof(DataType);  //解密次数

		for (int i = 0; i < num; i++)
		{
			bufferOut[i] = (char)dealDeciphering(buffer[i], m_key.dkey, m_key.pkey);
		}
		fout.write((char*)bufferOut, num);
	}
	fin.close();
	fout.close();
}

Key RSA::getKeys()
 {
	DataType prime1 = getPrime();  //获取素数
	DataType prime2 = getPrime();
	while (prime1 == prime2)
	{
		prime2 = getPrime();
	}

	m_key.pkey = getPublicKey(prime1, prime2);  //n
	DataType orla = Orla(prime1, prime2);  //f(n)
	m_key.ekey = getEKey(orla);  //e
	m_key.dkey = getDKey(m_key.ekey, orla);  //d

	cout << m_key.ekey << endl;  //////////////////////
	cout << m_key.dkey << endl;
	cout << m_key.pkey << endl;

	return m_key;
}

//工具

DataType RSA::getPrime()  //产生素数
{
	/*
	srand(time(NULL));  //小数据
	DataType prime;
	while (true)
	{
		prime = rand() % 100 + 2;  //随机产生一个2到99之间的数
		if (IsPrime(prime))
			return prime;
	}
	*/
	cout << "getPrime()" << endl;
	random::mt19937 gen(time(NULL));  //大数随机数发生器
	random::uniform_int_distribution<DataType> dist(0, DataType(1) << 256);  //随机数分布范围
 	DataType prime;
	while (true)
	{
 		prime = dist(gen);
		if (IsPrime(prime))
		{
			cout << "getPrime() finish!" << endl;
			return prime;
		}
	}
}
bool RSA::IsPrime(DataType data)  //判断素数
{
	/*
	if (data < 0)
		return false;
 	for (int i = 2; i <= sqrt(data); i++)
	{
		if (data % i == 0)
			return false;
	}
	return true;
	*/
	random::mt11213b gen(time(nullptr));
	if (miller_rabin_test(data, 25, gen))
	{
		if (miller_rabin_test((data - 1) / 2, 25, gen))
		{
			return true;
		}
	}
	return false;
}
DataType RSA::getPublicKey(DataType prime1, DataType prime2)  //求得公共密钥n
{
	return prime1 * prime2;
}
DataType RSA::Orla(DataType prime1, DataType prime2)  //欧拉函数求f(n)
{
	return (prime1 - 1)*(prime2 - 1);
}
DataType RSA::getEKey(DataType orla)  //获得加密密钥e
{
	/*
	srand(time(NULL));  //小型数据
	DataType ekey;
	while (true)
	{
		ekey = rand() % orla;
		if (1 < ekey && getGCD(ekey, orla) == 1)  //1 < e < orla 且 e与orly互质
			return ekey;
	}
	*/
	random::mt19937 gen(time(NULL));  //大数随机数发生器
	random::uniform_int_distribution<DataType> dist(2, orla);  //随机数分布范围
	DataType ekey;
	while (true)
	{
		ekey = dist(gen);
		if (getGCD(ekey, orla) == 1)
		{
			return ekey;
		}
	}
}
DataType RSA::getDKey(DataType ekey, DataType orla)	 //获得解密密钥d
{
	/*
	DataType dkey = 1;  //小数运算
	while (true)
	{
		if ((ekey * dkey) % orla == 1)
			return dkey;
		dkey++;
	}
	*/
	DataType x = 0, y = 0;
	getGCDway2(ekey, orla, x, y);  //根据ekey * d % f(n) = 1 求出通解
	return (x % orla + orla) % orla;  //进行转换，使密钥为一个较小的正解
}
DataType RSA::getGCD(DataType data1, DataType data2)  //求最大公约数
{
	DataType tmp;
	while (true)  //辗转相除法求最大公约数
	{
		if ((data1 % data2) == 0)
			return data2;
		tmp = data1;
		data1 = data2;
		data2 = tmp % data2;
	}
}
DataType RSA::getGCDway2(DataType a, DataType b, DataType& x, DataType& y)  //扩展欧几里得算法求最大公约数
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	DataType gcd = getGCDway2(b, a % b, x, y);  //获得该层递归的最大公约数
	DataType x1 = x, y1 = y;
	x = y1;  //根据数学推导公式得出解，并更新到传的&x和&y中
	y = x1 - a / b * y1;
	return gcd;
}