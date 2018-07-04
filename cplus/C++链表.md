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
