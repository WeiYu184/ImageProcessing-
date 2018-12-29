#pragma once
typedef struct StackNode
{
	int x;
	int y;    //���ص�λ��
	struct  StackNode  *next;
}StackNode, *LinkStack;

void CreatEmptyStack(LinkStack * S);
int IsEmpty(LinkStack S);
void PushStack(LinkStack *S, int x, int y);
void PopStack(LinkStack *S, int *x, int *y);