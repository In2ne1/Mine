#include"sort.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define BUFSIZE 10000

void printArray(int* src, int n)   //打印函数
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("%d ", src[i]);
	}
	printf("\n");
}

int main()
{
#if 1    //十个数的数组排序
	int src[10] = { 3,1,6,7,9,2,4,8,10,5 };  
	
	InsertSort(src, 10);  //插入排序
	printArray(src, 10);
	printf("\n");
	
	ShellSort(src, 10);    //希尔排序
	printArray(src, 10);
	printf("\n");

	QuickSort(src, 10);   //快速排序 4种方法
	printArray(src, 10);
	printf("\n");

	MergeSort(src, 10);   //归并排序
	printArray(src, 10);
	printf("\n");

	PickSort(src, 10);    //选择排序
	printArray(src, 10);
	printf("\n");

	BubbleSort(src, 10);   //冒泡排序
	printArray(src, 10);
	printf("\n");
	

#else   //一万个数字的数组排序
	srand(time(NULL));  //设置随机数种子

	int src[BUFSIZE];
	int i;
	for (i = 0; i < BUFSIZE; i++)
	{
		src[i] = rand() % 5000 + 1;  //使src中的范围是1到5000
	}

	InsertSort(src, BUFSIZE);  //插入排序
	printArray(src, BUFSIZE);
	printf("\n");
	
	ShellSort(src, BUFSIZE);  //希尔排序
	printArray(src, BUFSIZE);
	printf("\n");

	QuickSort(src, BUFSIZE);  //快排
	printArray(src, BUFSIZE);
	printf("\n");

	PickSort(src, BUFSIZE);    //选择排序
	printArray(src, BUFSIZE);
	printf("\n");

	BubbleSort(src, BUFSIZE);   //冒泡排序
	printArray(src, BUFSIZE);
	printf("\n");


#endif
	system("pause");
	return 0;
}


