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

BTNode* BinaryTreeCreate(BTDataType* src); //构造树
void BinaryTreePrevOrder(BTNode* root); //前序遍历
void BinaryTreeInOrder(BTNode* root);   //中序遍历
void BinaryTreePostOrder(BTNode* root); //后序遍历

void BinaryTreeDestory(BTNode* root);  //销毁树


int BinaryTreeSize(BTNode* root);   //树有多少个节点
int BinaryTreeLeafSize(BTNode* root);  //多少个叶子节点

#endif /*_BTREE_H_*/
