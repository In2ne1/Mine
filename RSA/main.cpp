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
//	cout << "原始数据:" << origin << endl;
//
//	DataType ecount = rsa.dealEncryption(origin, key.ekey, key.pkey);
//	cout << "加密:" << ecount << endl;
//
//	DataType dcount = rsa.dealDeciphering(ecount, key.dkey, key.pkey);
//
//
//	cout << "解密:" << dcount << endl;
//}

//void test2()  //boost库大数测试
//{
//	multiprecision::cpp_int ci("102416256963243233514586521345");
//	cout << ci + 1 << endl;
//}


int main()
{
	//test1();
	RSA rsa;

	rsa.Encryption("1.txt", "1e.txt");  //文件加密测试
	rsa.Deciphering("1e.txt", "1d.txt");


	//test2();


	system("pause");
	return 0;
}