#include"stdafx.h"
#include"List.h"
#include<stdlib.h>
void InitRegionList(RegionList *L)
{
	(*L) = (LRegion *)malloc(sizeof(LRegion));            //生成头结点       //头结点的x存储所有面积大于50的连通区域的个数                         
	(*L)->next = NULL;
	(*L)->x = 0;
}//初始化一个带头结点的空链表
void ListInsert(RegionList *L, int  x,int y, int flag)
{
	RegionList p = (*L)->next;
	RegionList newLNode = (LRegion *)malloc(sizeof(LRegion));//生成一个新节点
	newLNode->x = x;
	newLNode->y = y;
	newLNode->flag = flag;
	newLNode->are = 0;
	newLNode->perimeter = 0;
	newLNode->next = p;
	(*L)->next = newLNode;                             //将新节点插入到表头
}//在表头插入一个新的节点

//删除节点
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

