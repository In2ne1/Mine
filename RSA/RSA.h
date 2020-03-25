#pragma once
#include<boost/multiprecision/cpp_int.hpp>
#include<boost/multiprecision/random.hpp>  //大数随机数

//typedef long DataType;  //小数据加密
typedef boost::multiprecision::int1024_t DataType;

struct Key
{
	DataType ekey;  //加密密钥
	DataType dkey;  //解密密钥
	DataType pkey;  //公密密钥
};

/*
	获取素数
	求n
	欧拉函数求f(n)
	求加密密钥e     1 < e < f(n) 且 e与f(n)互质
	求解密密钥d
*/

class RSA
{
private:
	Key m_key;

	Key getKeys();//获取所有密钥

	DataType dealEncryption(DataType data, DataType ekey, DataType pkey);  //加密函数
	DataType dealDeciphering(DataType data, DataType dkey, DataType pkey);  //解密函数
	
	DataType getPrime();  //产生素数
	bool IsPrime(DataType data);  //判断是否为素数
	DataType getPublicKey(DataType prime1, DataType prime2);  //求得公共密钥n
	DataType Orla(DataType prime1, DataType prime2);  //欧拉函数求f(n)
	DataType getEKey(DataType orla);  //获得加密密钥e
	DataType getDKey(DataType ekey, DataType orla);	 //获得解密密钥d
	DataType getGCD(DataType data1, DataType data2);	 //求最大公约数
	DataType getGCDway2(DataType a, DataType b, DataType& x, DataType& y);  //扩展大数求最大公约数

public:

	RSA();  //构造函数

	//接口
	void Encryption(const char* filename, const char* fileout);  //加密函数接口
	void Deciphering(const char* filename, const char* fileout);  //解密函数接口

};