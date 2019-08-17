#ifndef _HEAP_H_
#define _HEAP_H_
typedef int HPDataType;

typedef struct Heap
{
	HPDataType* data;
	int size;
	int capacity;
}Heap;

void adjustDown(Heap* hp, int n);  //向下排序
void HeapInit(Heap* hp, HPDataType* a, int n); //数组转换成堆
void HeapDestory(Heap* hp);  //销毁
void HeapPush(Heap* hp, HPDataType x);  //插入
void HeapPop(Heap* hp);   //删除堆顶元素
HPDataType HeapTop(Heap* hp);  //输出堆顶元素
int HeapSize(Heap* hp);   //大小
void HeapPrint(Heap* hp);   //打印
int HeapEmpty(Heap* hp);  //返回堆是否为空
void HeapSort(Heap* hp);  //堆排序

#endif /*_HEAP_H_ */
