##C++链表
#include <iostream>
#include "show.hpp"
#include <unistd.h>
#include <assert.h>
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
