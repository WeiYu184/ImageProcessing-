#include"stdafx.h"
#include"Stack.h"
#include<stdlib.h>

void CreatEmptyStack(LinkStack *S)
{
	(*S) = (StackNode *)malloc(sizeof(StackNode));            //生成头结点                               
	(*S)->next = NULL;
}

int IsEmpty(LinkStack S)
{
	if (S->next == NULL)
		return 1;
	return 0;
}

void PushStack(LinkStack *S, int x, int y)
{
	LinkStack p = (StackNode *)malloc(sizeof(StackNode));
	p->x = x;
	p->y = y;
	p->next = (*S)->next;
	(*S)->next = p;
}

void PopStack(LinkStack *S, int *x, int *y)
{
	LinkStack p = (*S)->next;
	*x = p->x;
	*y = p->y;
	(*S)->next = p->next;
	free(p);
}