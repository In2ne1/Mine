#include"RSA.h"
#include<iostream>
#include<stdlib.h>
#include<boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost;



//void test1()
//{
//	RSA rsa;
//
//	Key key = rsa.getKeys();
//
//	DataType origin = 45;
//	cout << "ԭʼ����:" << origin << endl;
//
//	DataType ecount = rsa.dealEncryption(origin, key.ekey, key.pkey);
//	cout << "����:" << ecount << endl;
//
//	DataType dcount = rsa.dealDeciphering(ecount, key.dkey, key.pkey);
//
//
//	cout << "����:" << dcount << endl;
//}

//void test2()  //boost���������
//{
//	multiprecision::cpp_int ci("102416256963243233514586521345");
//	cout << ci + 1 << endl;
//}


int main()
{
	//test1();
	RSA rsa;

	rsa.Encryption("1.txt", "1e.txt");  //�ļ����ܲ���
	rsa.Deciphering("1e.txt", "1d.txt");


	//test2();


	system("pause");
	return 0;
}