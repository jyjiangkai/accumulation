##C++循环队列
队列是一种特殊的线性表。在队列中，仅允许在一端进行插入，而在另一端进行删除操作。允许插入的一端叫队尾(rear)，允许删除的一端叫队头(front)。
队列具有“先进先出”。

###顺序存储结构队列
####队列的初始化
typedef struct
{
	DataType queue[MAXLEN + 1];
	int front,rear;
}SeqQueue;

SeqQueue* initqueue()
{
	SeqQueue* sq = NULL;
	sq = new(SeqQueue);
	if (NULL == sq)
	{
		return NULL;
	}
	sq->front = 1;
	sq->rear = 1;
	
	return sq;
}

####入队
int addqueue(SeqQueue* sq, DataType x)
{
	if ((sq->front) == (((sq->rear)%MAXLEN) + 1))
	{
		printf("queue is full.\n");
		return 1;
	}
	sq->rear = (sq->rear)%MAXLEN + 1;
	sq->queue[sq->rear] = x;
	
	return 0;
}

####出队
int delqueue(SeqQueue* sq, DataType* px)
{
	if (sq->front == sq->rear)
	{
		printf("queue is empty.\n");
		return 1;
	}
	sq->front = (sq->front)%MAXLEN + 1;
	*px = sq->queue[sq->front];
	
	return 0;
}

###链式存储结构队列
####队列的初始化
链式队列节点数据类型：
typedef struct node
{
	DataType data;
	struct node* next;
}NodeType;

链式队列数据类型：
typedef struct
{
	NodeType* front;
	NodeType* rear;
}LinkQueue;

LinkQueue* initqueueL()
{
	LinkQueue* q = NULL;
	q = new(LinkQueue);
	if (NULL == q)
	{
		return NULL;
	}
	q->front = new(NodeType);
	if (NULL == q->front)
	{
		return NULL;
	}
	q->front->next = NULL;
	q->rear = q->front;
	return q;
}

####入队
int addqueueL(LinkQueue* q, DataType x)
{
	NodeType* s = NULL;
	s = new(NodeType);
	if (NULL == s)
	{
		return 1;
	}
	s->data = x;
	s->next = NULL;
	q->rear->next = s;
	q->rear = s;
	
	return 0;
}

####出队
int delqueueL(LinkQueue* q, DataType* px)
{
	NodeType* s = NULL;
	
	if (NULL == q->front->next)
	{
		printf("queue is empty.\n");
		return 1;
	}
	
	s = q->front->next;
	*px = s->data;
	q->front->next = s->next;
	if (NULL == q->front->next)
	{
		printf("queue is empty.\n");
		q->rear = q->front;
	}
	delete(s);
	return 0;
}