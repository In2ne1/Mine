#include"BTree.h"
#include<stdio.h>
#include<stdlib.h>

int main()
{
	BTNode * root = BinaryTreeCreate("ABD#GI##J###CE#HK###F##"); //先序树
	BinaryTreePrevOrder(root);
	printf("\n");
	BinaryTreeInOrder(root);
	printf("\n");
	BinaryTreePostOrder(root);
	printf("\n");

	int treesize, leafsize,levelksize;
	
	treesize = BinaryTreeSize(root);
	printf("%d\n", treesize);
	leafsize = BinaryTreeLeafSize(root);
	printf("%d\n", leafsize);

	BinaryTreeDestory(root);
	
	system("pause");
	return 0;
}
