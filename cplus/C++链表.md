##C++链表
#include <iostream>
#include <stdio.h>
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
	return head;
}

Node* push(Node* head, int* num)
{
	if (head == NULL)
	{
		head = new(Node);
		if (head == NULL)
		{
			return NULL;
		}
		head->data = *num;
		head->next = NULL;
		return head;
	}

	Node* s = NULL;
	s = new(Node);
	if (NULL == s)
	{
		return NULL;
	}

	s->data = *num;
	s->next = head;
	head = s;

	return head;
}

Node* pop(Node* head, int* num)
{
	Node* p = NULL;
	Node* q = NULL;
	p = head;

	if (head->data == *num)
	{
		head = head->next;
		return head;
	}
	while(NULL != p->next)
	{
		if (p->next->data == *num)
		{
			q = p->next;
			p->next = q->next;
			return head;
		}
		p = p->next;
	}
	return NULL;
}

//链表反转
Node* reverse(Node* head)
{
	if ((head == NULL)||(head->next == NULL))
	{
		return head;
	}

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
	head = p3;
	return head;
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

//链表元素值交换
void swap(Node* i, Node* j) 
{
	int temp = 0;
	temp = i->data;
	i->data = j->data;
	j->data = temp;
}

//链表快速排序
Node* getsep(Node* start, Node* end)
{
	if(NULL == start)
	{
		return NULL;
	}
	Node* i = start;
	Node* j = start->next;
	Node* mid = start;

	while(j!=end)
	{
		if ((j->data) < (mid->data))
		{
			i = i->next;
			swap(i, j);
		}
		j = j->next;
	}
	swap(i, mid);
	return i;
}

//链表快速排序
Node* quicksort(Node* start, Node* end)
{
	Node* head = start;
	Node* sep = NULL;
	if (start!=end)
	{
		sep = getsep(start, end);
		quicksort(start, sep);
		quicksort(sep->next, end);
	}
	return head;
}
