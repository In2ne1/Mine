#include"heap.h"
#include<stdio.h>
#include<stdlib.h>

void adjustDown(Heap* hp, int m) //向下排序算法，n为下标   大堆
{
	int cur = m;
	int n;
	while (cur * 2 + 1 < hp->size) //无左孩子则不进入循环
	{
		if (cur * 2 + 2 >= hp -> size)  //无右孩子，则直接和左孩子交换
		{
			n = cur * 2 + 1;  
		}
		else  
		{
			if (hp->data[cur * 2 + 1] > hp->data[cur * 2 + 2]) //左右孩子比较大小
			{
				n = cur * 2 + 1;  //大堆，左右孩子中谁大和谁换
			}
			else
			{
				n = cur * 2 + 2;
			}
		}
		if (hp->data[cur] < hp->data[n])  //和挑出较大的孩子比较，看是否需要交换
		{
			int tmp = hp->data[cur];
			hp->data[cur] = hp->data[n];
			hp->data[n] = tmp;
			cur = n;
		}
		else
		{
			break;
		}
	}
}

void HeapInit(Heap* hp, HPDataType* a, int n) // 初始化, n即为数组长度
{
	hp->capacity = n * 2; //容量为数组长度的2倍
	hp->size = n;
	hp->data = (HPDataType *)calloc(hp->capacity, sizeof(HPDataType));

	for (int i = 0; i < n; i++)
	{
		hp->data[i] = a[i]; //把数组拷贝到堆中
	}
	for(int i = n / 2 - 1; i > 0; i--)
	{
		adjustDown(hp, i); //向下调整算法，使数组变为堆，后才能进行堆排序操作
	}
}

void HeapDestory(Heap* hp)  //销毁
{
	if (hp->data)  //释放前先判空
	{
		free(hp->data);
	}
		hp->data = NULL;
		hp->capacity = 0;
		hp->size = 0;
}

void HeapPrint(Heap* hp)  //打印
{
	int i;
    int rn = 0;
    int bin = 1;
    for (i = 0; i < hp->size; ++i)  
    {
    	printf("%d ", hp->data[i]);
    	if (i == rn)   //按层打印
    	{
    		putchar('\n');
    		bin *= 2;
			rn += bin;
    	}
    }
	printf("\n\n");
}  

void HeapPush(Heap* hp, HPDataType x) //插入新数，向上调整算法，和父节点比较
{
	if (hp->size == hp->capacity)  //判断容量是否够用,不够则扩容
	{
		hp->capacity *= 2;
		hp->data = (HPDataType *)realloc(hp->data, hp->size, sizeof(HPDataType));
	}
	int cur = hp->size;  //要调整的下标
	hp->data[cur] = x;
	hp->size++;
	while (hp->data[(cur - 1) / 2])  //父节点存在则执行循环
	{
		if (hp->data[cur] > hp->data[(cur - 1) / 2]) //和父节点比较，判断是否要交换
		{
			int tmp = hp->data[cur];
			hp->data[cur] = hp->data[(cur - 1) / 2];
			hp->data[(cur - 1) / 2] = tmp;
			cur = (cur - 1) / 2;
		}
		else
			break;
	}

}

void HeapPop(Heap* hp)
{
	if (hp->size == 0)  //判空
	{
		return;
	}
	hp->size--;
	int tmp;
	//hp->data[0] = hp->data[hp->size];
	tmp = hp->data[0];   //堆顶元素与最后一个元素交换
	hp->data[0] = hp->data[hp->size];
	hp->data[hp->size] = tmp;
	 
	adjustDown(hp, 0);  //把交换过后的堆顶元素向下排序
}

HPDataType HeapTop(Heap* hp)  //返回堆顶元素
{
	if (hp->size == 0)
	{
		return (HPDataType)0; //强制类型转换
	}
	return hp->data[0];
}

void HeapSort(Heap* hp) //堆排序
{
	int tmp = hp->size;
	while (hp->size > 1)
	{
		HeapPop(hp);
	}
	hp->size = tmp;
}

int HeapSize(Heap* hp)
{
	return hp->size;
}

int HeapEmpty(Heap* hp)
{
	return hp->size == 0;  //为空
}

void adjustDown(Heap* hp, int n);
void HeapInit(Heap* hp, HPDataType* a, int n);
void HeapDestory(Heap* hp);
void HeapPush(Heap* hp, HPDataType x);
void HeapPop(Heap* hp);
HPDataType HeapTop(Heap* hp); //返回最顶端的值
int HeapSize(Heap* hp);
int HeapEmpty(Heap* hp);
void HeapSort(Heap* hp);
void HeapPrint(Heap* hp);
