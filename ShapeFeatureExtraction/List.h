#pragma once
typedef struct LRegion
{
	int x;
	int y;              //��ͨ�������Ͻ����ص�λ��
	int flag;           //��־
	int are;            //��ͨ��������
	int perimeter;      //��ͨ������ܳ�
	struct LRegion *next;
}LRegion, *RegionList; 

void InitRegionList(RegionList *L);

void ListInsert(RegionList *L, int x, int y,int flag);

void SearchRegion(RegionList L, int x, int y,int flag);

void DeleteRegion(RegionList *L);

