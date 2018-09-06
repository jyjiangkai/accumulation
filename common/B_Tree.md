// B_Tree.cpp : 定义控制台应用程序的入口点。
//
 
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
 
using namespace std;
 
#define  T 4
typedef struct B_Tree_Node
{
	int n;							//指示该结点的关键字个数
	int *keys;						//该结点关键字数组
	bool isLeaf;					//该结点是否是叶子结点
	struct B_Tree_Node **child ;    //该结点的所有孩子结点
	struct B_Tree_Node *p;		    //该结点的父结点
}B_Tree_Node, *p_B_Tree_Node;
 
B_Tree_Node *alloact_Node()
{
	B_Tree_Node *newNode = new B_Tree_Node;
	newNode->n = 0;
	newNode->isLeaf = true;
	newNode->keys = new int[2*T-1];
	newNode->child = new p_B_Tree_Node[2*T];
	newNode->p = NULL;
	for(int i=0;i<2*T;i++)
		newNode->child[i] = NULL;
	return newNode;
}
//从以当前结点为根结点的子树中，寻找k所在的结点，
/*
curNode 表示该当前子树的根结点，k 是要查找的关键字， index用来保存k在该结点中的索引
首先在当前结点中查找 ，若在该结点中，则直接返回该结点指针；如果没有找到，而且该结点是叶子结点，则返回NULL
否则，在它的适当的孩子结点中查找
*/
B_Tree_Node * searchNode(B_Tree_Node *curNode, int k, int &index)
{
	int i = 0;
	while(i<=curNode->n && k >curNode->keys[i]) 
		i++;
	if(i<curNode->n && k == curNode->keys[i])  //找到了k
	{
		index = i;
		return curNode;
	}
	if(curNode->isLeaf) //如果该结点是叶子结点，则k不存在
		return NULL;
	searchNode(curNode->child[i],k,index);
 
}
 
//splitNode_p是被分裂结点的父结点，i是索引为i的孩子为满，需要被分裂
//被分裂的结点是满的，那么它的n = 2*T - 1;被分裂为两个T-1个关键字的子结点，同时它的中间元素被提升到其父节点中
void BTree_Child_Split(B_Tree_Node *splitNode_p, int index_child)
{
	B_Tree_Node *newChild = alloact_Node();
	newChild->n = T-1;
	for(int i = 0;i<T-1;i++)
	{
		newChild->keys[i] = splitNode_p->child[index_child]->keys[T+i];
	}
	splitNode_p->child[index_child]->n = T-1;
	if(splitNode_p->child[index_child]->isLeaf!=true)  //如果它的第i个孩子不是叶子结点，则将它的后T个孩子也送给newChild
	{
		newChild->isLeaf = false;
		for(int i=0;i<T-1;i++)
			newChild->child[i] = splitNode_p->child[i+T];
	}
	//将newChild 添加为splitNode_p的第i+1个孩子结点，将中间关键字提升到它中
	for(int i = splitNode_p->n; i>=index_child;i--)
	{
		splitNode_p->child[i+1] = splitNode_p->child[i];
	}
	splitNode_p->n++;
	splitNode_p->child[index_child+1] = newChild;
	for(int i = splitNode_p->n-1; i>=index_child;i--)
	{
		splitNode_p->keys[i+1] = splitNode_p->keys[i];
	}
	splitNode_p->keys[index_child] = splitNode_p->child[index_child]->keys[T-1];
}
 
void BTree_Insert_NonFull(B_Tree_Node *nonfull, int k)
{
	int i = nonfull->n - 1;
	if(nonfull->isLeaf)
	{
		while(i>=0&&k<nonfull->keys[i])
		{
			nonfull->keys[i+1] = nonfull->keys[i];
			i--;
		}
		i = i+1;
		(nonfull->n)++;
		nonfull->keys[i] = k;
	}
	else
	{
		while(i>=0&&k<nonfull->keys[i])
			i--;
		i = i+1;
		if(nonfull->child[i]->n == 2*T-1)
		{
			BTree_Child_Split(nonfull,i);
			if(k>nonfull->keys[i])
				i = i+1;
		}
		BTree_Insert_NonFull(nonfull->child[i],k);
	}
}
 
//在B_Tree中加入新的关键字，主要由BTree_Insert_NonFull来实现，确保每次插入时所访问的结点都是非满结点;
//首先，若根结点为满，则分裂根结点
void BTree_Insert_Node(p_B_Tree_Node *root,int k)
{
	B_Tree_Node *p = *root;
	if(p->n == 2*T - 1) //如果根结点满
	{
		B_Tree_Node *newRoot =alloact_Node();
		newRoot->child[0] = (*root);
		newRoot->isLeaf = false;
		*root = newRoot;
		
		BTree_Child_Split(newRoot,0);
		BTree_Insert_NonFull(newRoot,k);
	}
	else 
		BTree_Insert_NonFull(*root,k);
}
 
void printBFS(B_Tree_Node *t)
{
	if(NULL == t)
		return;
 
	//输出当前节点所有关键字
	cout << "[";
	for(int i = 0;i < t->n;++i)
	{
		cout << t->keys[i];
		if(t->n - 1 != i)
			cout << " ";
	}
	cout << "]" << endl;
 
	//递归输出所有子树
	for(int i = 0;i <= t->n;++i)
		printBFS(t->child[i]);
 
}
 
//delete
/*
subNode,当前结点，以它为根的树中删除k
1.看k是否存在于以subNode为根的子树中，不存在则返回；存在则继续
2.看k是否存在于当前结点：（1）存在且该节点为叶子节点，则直接删除k;(2)存在且该节点为内部节点，分情况讨论；（3）不存在，则找出以它的孩子x.ci为根的子树中包含k,
Different Case:
1.存在于当前结点，且该结点为内部结点：
case 1:该结点中前于k的子结点y中有至少包含T个元素，则找出k'替换k,并递归地删除k';
case 2:该结点中前于k的子结点z中有只包含T-1个元素，但大于k的子结点中有至少包含T个元素，同上找出k'替换k,递归地删除k';
case 3：以上两个子结点均只包含T-1个元素，那么将k与z子结点中的元素均归并到y中。再递归删除k
2.k不存在于当前的结点中,存在于以它的孩子x.ci为根的子树中。
case 1:若x.ci中至少包含有T个元素，递归找删除k
case 2:若x.ci中只有T-1个元素，而x.ci-1或x.ci+1中至少有T个元素，刚将x中合适的元素取出来给x.ci,x.ci-1或x.ci+1中合适的元素取出来给x,递归删除k
case 3：若x.ci-1和x.ci+1中均只有T-1个元素，那么将x.ci-1与x.ci合并，或另外两个合并，并将x中合适的元素作为它们的中间关键字。
*/
void BTree_delete_key(B_Tree_Node *subNode, int k)
{
	int index = 0;
	B_Tree_Node *deleteNode = NULL;
	if((deleteNode = searchNode(subNode,k,index)) == NULL)
		return;
	int keyIndex = -1;
	for(int i=0;i<subNode->n;i++)
	{
		if(k == subNode->keys[i])
		{
			keyIndex = i;
			break;
		}
	}
	 //如果在当前结点，且当前结点为叶子结点，则直接删除k
	//OK******************************
	if(keyIndex != -1 && subNode->isLeaf)
	{
		for(int i=keyIndex;i<subNode->n-1;i++)
		{
			subNode->keys[i] = subNode->keys[i+1];
		}
		(subNode->n)--;
	}
	//如果在当前结点中，且当前结点不为叶子结点
	else if(keyIndex != -1 && subNode->isLeaf!= true)
	{
		B_Tree_Node *processorNode = subNode->child[keyIndex];
		B_Tree_Node *succssorNode = subNode->child[keyIndex+1];
		//如果小于k的孩子结点关键字数大于T
		if(processorNode->n >= T)
		{
			int k1 = processorNode->keys[processorNode->n-1];
			subNode->keys[keyIndex] = k1;
			BTree_delete_key(processorNode,k1);
		}
		//如果大于k的孩子结点关键字数大于T
		else if(succssorNode->n >=T)
		{
			int k1 = succssorNode->keys[0];
			subNode->keys[keyIndex] = k1;
			BTree_delete_key(succssorNode,k1);
		}
		//如果两个孩子结点关键字数均不大于T,则将k与右孩子结点的关键字归并到左孩子中
		else
		{
			for(int j=0;j<T-1;j++)
			{
				//processorNode->keys[T-1] = k;这里最好不要使用T表示，因为如果是根结点的话，可能它的关键字数不为T
				processorNode->keys[processorNode->n] = k;
				processorNode->keys[processorNode->n+1+j] = succssorNode->keys[j];
			}
			
			processorNode->n = 2*T -1 ;
			//将subNode->child[keyIndex+1]的孩子传给左邻孩子
			if(!processorNode->isLeaf)
			{
				for(int j=0;j<T;j++)
				{
					processorNode->child[T+j] = succssorNode->child[j];
				}
			}
			//修改subNode中的key值
			for(int j = keyIndex;j<subNode->n-1;j++)
			{
				subNode->keys[j] = subNode->keys[j+1];
			}
			subNode->n = subNode->n - 1;
			delete succssorNode;
			BTree_delete_key(processorNode,k);
 
			
		}
 
	}
	else if(keyIndex == -1) //不在当前结点中
	{
		int childIndex = 0;
		B_Tree_Node *deleteNode = NULL;
		//寻找合适的子孩子，以该子孩子为根的树包含k
		for(int j = 0;j<subNode->n;j++)
		{
			if(k<subNode->keys[j])
			{
				childIndex = j;
				deleteNode = subNode->child[j];
				break;
			}
		}
		//如果该子孩子的关键字数小于T,考虑那两种情况
		if(deleteNode->n <= T-1)
		{
			//deleteNode的左兄弟结点
			B_Tree_Node *LeftNode = subNode->child[childIndex-1];
			//deleteNode的右兄弟结点
			B_Tree_Node *RightNode = subNode->child[childIndex+1];
			//如果左兄弟结点关键字数大于T,将父结点中的第childIndex-1个元素送给deleteNode,将Left中的最大元素送给父结点，
			if(childIndex>=1 && LeftNode->n >= T)
			{
				for(int i = deleteNode->n;i>0;i--)
				{
					deleteNode->keys[i] = deleteNode->keys[i-1];
				}
				deleteNode->keys[0] = subNode->keys[childIndex];
				subNode->keys[childIndex] = LeftNode->keys[LeftNode->n - 1];
				(LeftNode->n)--;
				(deleteNode->n)++;
				BTree_delete_key(deleteNode,k);
			}
			//如果右兄弟关键字大于T,将父结点中的第childIndex个元素送给deleteNode,将Right中的最小元素送给父结点，
			else if(childIndex<subNode->n && RightNode->n >= T)
			{
				deleteNode->keys[deleteNode->n] = subNode->keys[childIndex];
				subNode->keys[childIndex] = RightNode->keys[0];
				for(int i=0;i<RightNode->n-1;i++)
					RightNode[i] = RightNode[i+1];
				(RightNode->n)--;
				(deleteNode->n)++;
				BTree_delete_key(deleteNode,k);
			}
			//如果左兄弟和右兄弟的关键字数均不在于T,则将左兄弟或右兄弟与其合并
			else 
			{
				if(childIndex>=1)//左兄弟存在，合并
				{
					//将keys合并
					for(int i=0;i<deleteNode->n;i++)
					{
						LeftNode->keys[LeftNode->n+i] = deleteNode->keys[i];
					}
					//如果非叶子结点，则将叶子也合并
					if(!deleteNode->isLeaf)
					{
						for(int i=0;i<deleteNode->n+1;i++)
						{
							LeftNode->child[LeftNode->n+1+i] = deleteNode->child[i];
						}
 
					}
					LeftNode->n = LeftNode->n + deleteNode->n;
 
					//调整subNode的子节点
					for(int i = childIndex;i<subNode->n;i++)
					{
						subNode->child[i] = subNode->child[i+1];
					}
					BTree_delete_key(LeftNode,k);
				}
				else //合并它和右兄弟
				{
					//将keys合并
					for(int i=0;i<RightNode->n;i++)
					{
						deleteNode->keys[i+deleteNode->n] = RightNode->keys[i];
					}
					//如果非叶子结点，则将叶子合并
					if(!deleteNode->isLeaf)
					{
						for(int i = 0;i<RightNode->n+1;i++)
						{
							deleteNode->child[deleteNode->n + 1 + i] = RightNode->child[i];
						}
					}
					deleteNode->n = deleteNode->n + RightNode->n;
 
					//调整subNode的子节点
					for(int i = childIndex+1;i<subNode->n;i++)
					{
						subNode->child[i] = subNode->child[i+1];
					}
					BTree_delete_key(deleteNode,k);
				}
			}
			
		}
		BTree_delete_key(deleteNode,k);
	}
}
 
void createBTree(p_B_Tree_Node *root)
{
	int a[] = {12,1,9,2,0,11,7,19,4,15,18,5,14,13,10,16,6,3,8,17};
	for(int i = 0;i<20;i++)
	{
		BTree_Insert_Node(root,a[i]);
		printBFS(*root);
	}
}
 
 
int _tmain(int argc, _TCHAR* argv[])
{
	B_Tree_Node *root = alloact_Node();
	createBTree(&root);
	cout<<"B_Tree after delete 12:"<<endl;
	BTree_delete_key(root,12);
	printBFS(root);
	cout<<"B_Tree after delete 1:"<<endl;
	BTree_delete_key(root,1);
	printBFS(root);
	cout<<"B_Tree after delete 9:"<<endl;
	BTree_delete_key(root,9);
	printBFS(root);
	cout<<"B_Tree after delete 2:"<<endl;
	BTree_delete_key(root,2);
	printBFS(root);
 
	return 0;
}
 
