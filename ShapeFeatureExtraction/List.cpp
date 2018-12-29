#include"stdafx.h"
#include"List.h"
#include<stdlib.h>
void InitRegionList(RegionList *L)
{
	(*L) = (LRegion *)malloc(sizeof(LRegion));            //����ͷ���       //ͷ����x�洢�����������50����ͨ����ĸ���                         
	(*L)->next = NULL;
	(*L)->x = 0;
}//��ʼ��һ����ͷ���Ŀ�����
void ListInsert(RegionList *L, int  x,int y, int flag)
{
	RegionList p = (*L)->next;
	RegionList newLNode = (LRegion *)malloc(sizeof(LRegion));//����һ���½ڵ�
	newLNode->x = x;
	newLNode->y = y;
	newLNode->flag = flag;
	newLNode->are = 0;
	newLNode->perimeter = 0;
	newLNode->next = p;
	(*L)->next = newLNode;                             //���½ڵ���뵽��ͷ
}//�ڱ�ͷ����һ���µĽڵ�

//ɾ���ڵ�
void DeleteRegion(RegionList *L)
{
	LRegion *p = (*L)->next;
	(*L)->next = p->next;
	p->next = NULL;
	free(p);
}
void SearchRegion(RegionList L, int x, int y,int flag)
{
	RegionList p = L->next;
	while (p!=NULL)
	{
		if (p->x == x&&p->y == y)
		{
			p->flag = flag;
			break;
		}
		p = p->next;
	}
}

