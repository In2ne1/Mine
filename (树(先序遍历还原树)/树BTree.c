#include"BTree.h"
#include<stdio.h>
#include<stdlib.h>

BTNode* BinaryTreeCreate(BTDataType* src)  //int* pi //src树的节点，pi记录访问第几个字符
{
	static int s_n = 0;  //局部静态变量记录访问第几个字符
	if (src[s_n] == '#')
	{
		s_n++;
		return NULL;
	}
	BTNode* cur = (BTNode*)malloc(sizeof(BTNode)); //开辟存放节点的空间
	cur->_data = src[s_n];
	s_n++;

	//递归
	cur->lchild = BinaryTreeCreate(src);
	cur->rchild = BinaryTreeCreate(src);
	return cur;
}
