#include"sort.h"


void InsertSort(int* src, int n)   /插入排序
{
	int i, j;
	int tmp;
	for (int i = 1; i < n; i++)
	{
		tmp = src[i];
		for (j = i; j > 0 && src[j - 1] > tmp; j--)
		{
			src[j] = src[j - 1];
		}
		src[j] = tmp;
	}
}


void ShellSort(int* src, int n   //希尔排序
{
	int i, j, k;
	int tmp, gap;
	for (gap = n / 2; gap; gap /= 2)  //2改为3会有一些速度提升
	{
		for (k = 0; k < gap; k++)
		{
			for (int i = gap + k; i < n; i += gap)
			{
				tmp = src[i];
				for (j = i; j >= gap && src[j - gap] > tmp; j -= gap)
				{
					src[j] = src[j - gap];
				}
				src[j] = tmp;
			}
		}
	}
}


void dealMergeSort(int* src, int* tmp, int start, int end)   //归并排序函数实现
{
	if (start >= end)
	{
		return;
	}
	int mid = (start + end) / 2;    //分治
	dealMergeSort(src, tmp, start, mid);
	dealMergeSort(src, tmp, mid + 1, end);
	int a = start;
	int b = mid + 1;
	int c = start;
	while (a <= mid && b <= end)  //两个已排序分好的数组,从小到大赋到tmp中
	{
		if (src[a] < src[b])
		{
			tmp[c] = src[a];
			a++;
		}
		else
		{
			tmp[c] = src[b];
			b++;
		}
		c++;
	}
	for (; a <= mid; a++, c++)   //若a剩余，a中剩余数据直接移到tmp中
	{
		tmp[c] = src[a];
	}
	for (; b <= end; b++, c++)   //若b剩余，b中剩余数据直接移到tmp中
	{
		tmp[c] = src[b];
	}
	for (int i = start; i <= end; i++)   //最后把tmp赋给src
	{
		src[i] = tmp[i];
	}
}

void MergeSort(int* src, int n)   //归并排序函数入口
{
	int* tmp = (int*)malloc(n * sizeof(int));
	dealMergeSort(src, tmp, 0, n - 1);
	free(tmp);
}


//快排
int doublePointerWay1(int* src, int start, int end) //双指针排序
{
	int a = start, b = end;
	int flag = 0;    //flag为0即为b指针动，为1即a指针动
	while (src[b] > src[a])   //找到比a指针的基准值小的值，即为b指针指向的值
	{                         //a,b交汇，不执行下面的while
		b--;
	}
	while (a < b)  //找到需要交换的值执行循环
	{
		swapArgs(src + b, src + a);  //交换
		flag = !flag;

		while (src[b] >= src[a])  //交换过后指针移动
		{
			flag ? a++ : b--;   //flag为1时a指针向后动,为0时b指针向前动
		}
	}
	return flag ? b : a; //flag为1时a指针动，返回b指针的值；反之b指针动，返回a指针的值
}

int doublePointerWay2(int * src, int start, int end)  //前后指针法排序，基准值变为mid位置的值
{
	int a = start, b = end - 1;
	int mid = (start + end) / 2;   //基准值
	swapArgs(src + mid, src + end);
	while (a <= b)
	{
		while (src[a] <= src[end] && a < end)
		{
			a++;
		}
		while (src[b] >= src[end] && b > 0)
		{
			b--;
		}
		if (a == b && (a == 0 || a == end))
			break;
		if (a < b)  //指针交错跳出循环，但还需再交换一次
			swapArgs(src + a, src + b);
	}
	swapArgs(src + a, src + end);
	return a;
}

int doublePointerWay3(int * src, int start, int end)  //挖坑法排序
{
	int a = start, b = end;
	int mid = src[a];  //基准值 
	while (a < b)
	{
		while (src[b] > mid && start < b)  //b指针寻找比基准值小的数
		{
			b--;
		}
		if (a < b)
		{
			src[a] = src[b];  //值覆盖
			a++;
		}
	
		while (src[a] <= mid && a < b)  //a指针寻找比基准值大的数
		{
			a++;
		}
		if(a < b)
		{
			src[b] = src[a];   //值覆盖
			b--;
		}
	}
	src[a] = mid;   //跳出循环时,填这次循环中的最后一个坑
	
	return a;
}

int doublePointerWay4(int * src, int start, int end)   //三数取中法，霍尔排序
{
	int a = start + 1, b = end - 2;   //最后一个位置保护基准值
	int mid = (start + end) / 2;
	if (src[start] > src[mid])   //三数排序
	{
		swapArgs(src + start, src + mid);
	}
	if (src[mid] > src[end])   
	{
		swapArgs(src + mid, src + end);
	}
	if (src[start] > src[mid])   
	{
		swapArgs(src + start, src + mid);
	}
	if (end - start <= 2)   //如果只有三个元素，直接排好返回mid值
	{
		return mid;
	}
	swapArgs(src + mid, src + end - 1);

	while (a <= b)
	{
		while (src[a] <= src[end - 1] && a < end - 1)
		{
			a++;
		}
		while (src[b] >= src[end - 1] && b > 1)
		{
			b--;
		}
		if (a == b && (a == 1 || a == end - 1))
		{
			break;
		}
		if (a < b)  //指针交错跳出循环，但还需再交换一次
		{
			swapArgs(src + a, src + b);
		}
	}
	swapArgs(src + a, src + end - 1);
	return a;
}

//指针排序用到的函数
void swapArgs(int* pa, int* pb) //交换函数
{
	int tmp;
	tmp = *pa;
	*pa = *pb;
	*pb = tmp;
}

void dealQuickSort(int* src, int start, int end)  //快排函数通道
{
	 int mid;
	 if (start < end)
	 {
		 //mid = doublePointerWay1(src, start, end);
		 //mid = doublePointerWay2(src, start, end);
		 //mid = doublePointerWay3(src, start, end);
		 mid = doublePointerWay4(src, start, end);
		 dealQuickSort(src, start, mid - 1);  //递归调用排序
		 dealQuickSort(src, mid + 1, end);
	 }
}

void QuickSort(int * src, int n)  //快排接口
{
	dealQuickSort(src, 0, n - 1);
}


void PickSort(int* src, int n)   //选择法排序
{
	int i, j;
	int tmp;
	for (i = 0; i < n - 1; i++)  //要交换的位置
	{
		for (j = i; j < n; j++)  //找到最小的值
		{
			if (src[j] < src[i])
			{
				tmp = src[i];
				src[i] = src[j];
				src[j] = tmp;
			}
		}
	}
}


void BubbleSort(int* src, int n)   //冒泡排序
{
	int i, j;
	int tmp;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - 1 - i; j++)
		{
			if (src[j] > src[j + 1])
			{
				tmp = src[j];
				src[j] = src[j + 1];
				src[j + 1] = tmp;
			}
		}
	}
}
