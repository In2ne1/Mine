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

void BinaryTreePrevOrder(BTNode* root)   //先序，越下的节点越先遍历，使用递归
{
	if(root)
	{ 
	    putchar(root->data);
	    BinaryTreePrevOrder(root->lchild);
	    BinaryTreePrevOrder(root->rchild);
	}
}

void BinaryTreeInOrder(BTNode* root)  //中序
{
	if(root)
	{ 
	    BinaryTreeInOrder(root->lchild);
	    putchar(root->data);
	    BinaryTreeInOrder(root->rchild);
	}
}

void BinaryTreePostOrder(BTNode* root)  //后序
{
	if (root)
	{
		BinaryTreePostOrder(root->lchild);
		BinaryTreePostOrder(root->rchild);
		putchar(root->data);
	}
}

void BinaryTreeDestory(BTNode* root)  //后序销毁
{
	if (root)
	{
		BinaryTreeDestory(root->lchild);
		BinaryTreeDestory(root->rchild);
		free(root);
	}
}


int BinaryTreeSize(BTNode* root)  //树节点个数
{
	if (root)
	{
		return BinaryTreeSize(root->lchild) + BinaryTreeSize(root->rchild) + 1;
	}
	return 0;
}

int BinaryTreeLeafSize(BTNode* root)  //叶子节点个数
{
	if(root)
	{
		if (root->lchild == NULL && root->rchild == NULL)
			return BinaryTreeLeafSize(root->lchild) + BinaryTreeLeafSize(root->rchild) + 1;
		else
			return BinaryTreeLeafSize(root->lchild) + BinaryTreeLeafSize(root->rchild);
	}
	return 0;
}
