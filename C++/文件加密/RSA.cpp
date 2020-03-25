#include"RSA.h"
#include<iostream>
#include<time.h>
#include<fstream>
#include <boost/multiprecision/miller_rabin.hpp>
using namespace std;
using namespace boost;

#define NUMBER 256

RSA::RSA()  //���캯��
{
	getKeys();
}


DataType RSA::dealEncryption(DataType data, DataType ekey, DataType pkey)  //���ܺ���(e, n)
{
	/*
	return (DataType)pow(data, ekey) % pkey;  //������ᵼ���������
	a^b % c  //����ͬ�ඨ����иĽ�
	*/

	DataType Ai = data;  //ÿһ��
	DataType msgE = 1;
	while (ekey)
	{
		if (ekey & 1)
			msgE = (Ai * msgE) % pkey;  //��ǰ��� = (ǰһ���� * ��ǰ��) % c
		ekey >>= 1;  //����b
		Ai = (Ai * Ai) % pkey;  //����AiΪǰһ�����ĳ˻���ȡģ��Ai = [A(i - 1)*A(i - 1)] % c
	}
	return msgE;
}
DataType RSA::dealDeciphering(DataType data, DataType dkey, DataType pkey)  //���ܺ���(d, n)
{
	return dealEncryption(data, dkey, pkey);
}


//�ӿ�
void RSA::Encryption(const char* filename, const char* fileout)  //���ܺ����ӿ�
{
	ifstream fin(filename);  //���������ļ������󣬲�ѡ���Զ�������ʽ��
	ofstream fout(fileout);
	if (!fin.is_open())  //�ж��ļ��Ƿ��
		return;

	char* buffer = new char[NUMBER];  //���ö�ȡ�������ַ�
	DataType* bufferOut = new DataType[NUMBER];  //���ü��ܺ��ļ����ݵ�����ļ�
	while (!fin.eof())  //��ȡ�ļ��������м���
	{
		fin.read(buffer, NUMBER);  //��ȡҪ���ܵ��ļ����ݣ������뻺������
		int curNum = fin.gcount();  //���һ�ζ�ȡ�����ֽ���

		for (int i = 0; i < curNum; i++)  //���ܣ��������ܺ����Ϣ������ļ���
		{
			bufferOut[i] = dealEncryption(buffer[i], m_key.ekey, m_key.pkey);
		}
		fout.write((char*)bufferOut, curNum * sizeof(DataType));
	}
	fin.close();
	fout.close();
}
void RSA::Deciphering(const char* filename, const char* fileout)  //���ܺ����ӿ�
{
	ifstream fin(filename);
	ofstream fout(fileout);
	if (!fin.is_open())
		return;
	DataType* buffer = new DataType[NUMBER];  //���ö�ȡ�����ļ�������
	char* bufferOut = new char[NUMBER];   //���ü��ܺ����������
	while (!fin.eof())
	{
		fin.read((char*)buffer, NUMBER * sizeof(DataType));  //��buffer�ж�ȡ��������
		int num = fin.gcount();  //��ȡ���һ�ζ�ȡ���ֽ���
		num /= sizeof(DataType);  //���ܴ���

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
	DataType prime1 = getPrime();  //��ȡ����
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

//����

DataType RSA::getPrime()  //��������
{
	/*
	srand(time(NULL));  //С����
	DataType prime;
	while (true)
	{
		prime = rand() % 100 + 2;  //�������һ��2��99֮�����
		if (IsPrime(prime))
			return prime;
	}
	*/
	cout << "getPrime()" << endl;
	random::mt19937 gen(time(NULL));  //���������������
	random::uniform_int_distribution<DataType> dist(0, DataType(1) << 256);  //������ֲ���Χ
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
bool RSA::IsPrime(DataType data)  //�ж�����
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
DataType RSA::getPublicKey(DataType prime1, DataType prime2)  //��ù�����Կn
{
	return prime1 * prime2;
}
DataType RSA::Orla(DataType prime1, DataType prime2)  //ŷ��������f(n)
{
	return (prime1 - 1)*(prime2 - 1);
}
DataType RSA::getEKey(DataType orla)  //��ü�����Կe
{
	/*
	srand(time(NULL));  //С������
	DataType ekey;
	while (true)
	{
		ekey = rand() % orla;
		if (1 < ekey && getGCD(ekey, orla) == 1)  //1 < e < orla �� e��orly����
			return ekey;
	}
	*/
	random::mt19937 gen(time(NULL));  //���������������
	random::uniform_int_distribution<DataType> dist(2, orla);  //������ֲ���Χ
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
DataType RSA::getDKey(DataType ekey, DataType orla)	 //��ý�����Կd
{
	/*
	DataType dkey = 1;  //С������
	while (true)
	{
		if ((ekey * dkey) % orla == 1)
			return dkey;
		dkey++;
	}
	*/
	DataType x = 0, y = 0;
	getGCDway2(ekey, orla, x, y);  //����ekey * d % f(n) = 1 ���ͨ��
	return (x % orla + orla) % orla;  //����ת����ʹ��ԿΪһ����С������
}
DataType RSA::getGCD(DataType data1, DataType data2)  //�����Լ��
{
	DataType tmp;
	while (true)  //շת����������Լ��
	{
		if ((data1 % data2) == 0)
			return data2;
		tmp = data1;
		data1 = data2;
		data2 = tmp % data2;
	}
}
DataType RSA::getGCDway2(DataType a, DataType b, DataType& x, DataType& y)  //��չŷ������㷨�����Լ��
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	DataType gcd = getGCDway2(b, a % b, x, y);  //��øò�ݹ�����Լ��
	DataType x1 = x, y1 = y;
	x = y1;  //������ѧ�Ƶ���ʽ�ó��⣬�����µ�����&x��&y��
	y = x1 - a / b * y1;
	return gcd;
}