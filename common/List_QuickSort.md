//单链表快速排序

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

using namespace std;

typedef struct node
{
	int data;
	struct node *next;
}Node;

Node* init()
{
	Node* head = NULL;
	head = (Node*)new(Node);
	if(NULL == head)
	{
		return NULL;
	}
	head->data = 0;
	head->next = NULL;
	return head;
}

int push(Node* head, int* num)
{
	Node* s = NULL;
	s = new(Node);
	if (NULL == s)
	{
		return 1;
	}

	s->data = *num;
	s->next = head->next;
	head->next = s;

	return 0;
}

int pop(Node* head, int* num)
{
	Node* p = NULL;
	Node* q = NULL;
	p = head;

	while(NULL != p->next)
	{
		if (p->next->data == *num)
		{
			q = p->next;
			p->next = q->next;
			return 0;
		}
		p = p->next;
	}
	return 1;
}

//链表反转
Node* reverse(Node* head)
{
	Node* p1 = head->next;
	Node* p2 = head->next;
	Node* p3 = NULL;

	head->next = NULL;
	p3 = head;

	while(p1 != NULL)
	{
		p1 = p1->next;
		p2->next = p3;
		p3 = p2;
		p2 = p1;
	}
	return p3;
}

void show(Node* head)
{
	Node* p = NULL;
	p = head;

	while(NULL != p)
	{
		cout<<"==="<<p->data<<"==="<<endl;
		p = p->next;
	}
}

void QuickSort(Node* head, Node* start, Node* end);

int main()
{
	Node* head = init();
	for(int i=5;i>0;i--)
	{
		push(head, &i);
	}
	show(head);

	QuickSort(head, head, NULL);

	show(head);

	system("pause");
	return 0;
}

void QuickSort(Node* head, Node* start, Node* end)  /* 对表list[s]~list[t]进行快速排序 */
{
	if (head==NULL || start==end)
	{
		return;
	}
	Node* mid;
	Node* i = head;
	Node* j = i->next;
	int temp = 0;
	while(j->next != NULL)
	{
		if(i->data > j->data)
		{
			i = i->next;
			temp = i->data;
			i->data = j->data;
			j->data = temp;
		}
		j = j->next;
	}
	system("pause");

	QuickSort(head, start, i);  /* 对划分得到的两个子表继续进行快速排序 */
	QuickSort(head, i->next, NULL);
}