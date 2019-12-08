#ifndef _SORT_H_
#define _SORT_H_

void InsertSort(int* src, int n);  //插入排序，时间复杂度O(n)到O(n^2)

void ShellSort(int* src, int n);   //希尔排序

void dealMergeSort(int* src, int* tmp, int start, int end); //归并排序，时间复杂度O(n*log(n)),共log(n)层，每层复杂度是n。
void MergeSort(int* src, int n); //归并排序外界接口 从简

void QuickSort(int * src, int n);  //快排时间复杂度O(n*log(n))到O(n^2)
void dealQuickSort(int* serc, int start, int end);  //快排函数通道
void swapArgs(int* pa, int* pb); //交换函数
int doublePointerWay1(int* src, int start, int end); //双指针排序
int doublePointerWay2(int * src, int start, int end); //前后指针排序
int doublePointerWay3(int * src, int start, int end);  //挖坑排序
int doublePointerWay4(int * src, int start, int end);  //三数取中法 霍尔排序

void PickSort(int* src, int n);//选择法排序

void BubbleSort(int* src, int n);//冒泡排序

#endif /*_SORT_H_*/
