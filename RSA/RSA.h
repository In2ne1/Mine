#pragma once
#include<boost/multiprecision/cpp_int.hpp>
#include<boost/multiprecision/random.hpp>  //���������

//typedef long DataType;  //С���ݼ���
typedef boost::multiprecision::int1024_t DataType;

struct Key
{
	DataType ekey;  //������Կ
	DataType dkey;  //������Կ
	DataType pkey;  //������Կ
};

/*
	��ȡ����
	��n
	ŷ��������f(n)
	�������Կe     1 < e < f(n) �� e��f(n)����
	�������Կd
*/

class RSA
{
private:
	Key m_key;

	Key getKeys();//��ȡ������Կ

	DataType dealEncryption(DataType data, DataType ekey, DataType pkey);  //���ܺ���
	DataType dealDeciphering(DataType data, DataType dkey, DataType pkey);  //���ܺ���
	
	DataType getPrime();  //��������
	bool IsPrime(DataType data);  //�ж��Ƿ�Ϊ����
	DataType getPublicKey(DataType prime1, DataType prime2);  //��ù�����Կn
	DataType Orla(DataType prime1, DataType prime2);  //ŷ��������f(n)
	DataType getEKey(DataType orla);  //��ü�����Կe
	DataType getDKey(DataType ekey, DataType orla);	 //��ý�����Կd
	DataType getGCD(DataType data1, DataType data2);	 //�����Լ��
	DataType getGCDway2(DataType a, DataType b, DataType& x, DataType& y);  //��չ���������Լ��

public:

	RSA();  //���캯��

	//�ӿ�
	void Encryption(const char* filename, const char* fileout);  //���ܺ����ӿ�
	void Deciphering(const char* filename, const char* fileout);  //���ܺ����ӿ�

};