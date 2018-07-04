##C++栈
栈是一种受限定的线性表，限定其插入和删除操作只能在表的一端进行。

###顺序存储结构栈
####栈的初始化
typedef struct
{
	DataType stack[MAXLEN + 1];
	int top;
}StackType;

StackType* initstack()
{
	StackType* s = NULL;
	s = new(StackType);
	if (NULL == s)
	{
		return NULL;
	}
	s->top = 0;
	return s;
}

####判断空栈
int empty(StackType* s)
{
	if (s->top == 0)
	{
		return 0;
	}
	return 1;
}

####入栈
int push(StackType* s, DataType x)
{
	if (s->top == MAXLEN)
	{
		printf("stack is full.\n");
		return 1;
	}
	s->top ++;
	s->stack[s->top] = x;
	
	return 0;
}

####出栈
int pop(StackType* s, DataType* px)
{
	if (s->top == 0)
	{
		printf("stack is empty.\n");
		return 1;
	}
	*px = s->stack[s->top --];
	
	return 0;
}

###链式存储结构栈
####栈的初始化
typedef struct node
{
	DataType data;
	struct node* next;
}NodeType;

NodeType* initstackL()
{
	NodeType* head = NULL;
	head = new(NodeType);
	if (NULL == head)
	{
		return NULL;
	}
	head->data = DataType(0);
	head->next = NULL;
	return head;
}

####判断空栈
int emptyL(NodeType* head)
{
	if (head->next == NULL)
	{
		return 0;
	}
	return 1;
}

####入栈
int pushL(NodeType* head, DataType x)
{
	NodeType* s = NULL;
	s = new(NodeType);
	if (NULL == s)
	{
		return 1;
	}
	s->data = x;
	s->next = head->next;
	head->next = s;
	
	return 0;
}

####出栈
int popL(NodeType* head, DataType* px)
{
	NodeType* s = NULL;
	if (NULL == head->next)
	{
		printf("stack is empty.\n");
		return 1;
	}
	s = head->next;
	*px = s->data;
	head->next = s->next;
	delete(s);
	
	return 0;
}
