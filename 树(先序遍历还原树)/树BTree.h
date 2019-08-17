#ifndef _BTREE_H_
#define _BTREE_H_
#include<stdio.h>
#include<stdlib.h>


typedef char BTDataType;
typedef struct BinaryTreeNode
{
	BTDataType _data;
	struct BinaryTreeNode* lchild;
	struct BinaryTreeNode* rchild;
}BTNode;


#endif /*_BTREE_H_*/
