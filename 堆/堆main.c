#include"heap.h"
#include<stdio.h>
#include<stdlib.h>


int main()
{
	int a;
	int data[10] = { 29,48,72,53,45,30,18,36,13,35 };
	Heap hp;
	HeapInit(&hp, data, 10);
	HeapPrint(&hp);
    
	adjustDown(&hp, 1);
	HeapPrint(&hp);
	
	HeapPush(&hp, 49);
	HeapPrint(&hp);
	
	HeapPop(&hp);
	HeapPrint(&hp);
	
	HeapSort(&hp);
	HeapPrint(&hp);
	a =  HeapTop(&hp); //返回最顶端的值
	printf("%d", a);
	HeapDestory(&hp);
	HeapPrint(&hp);
	
	system("pause");
	return 0;
}
