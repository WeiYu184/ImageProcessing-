#pragma once
typedef struct LRegion
{
	int x;
	int y;              //连通区域左上角像素的位置
	int flag;           //标志
	int are;            //连通区域的面积
	int perimeter;      //连通区域的周长
	struct LRegion *next;
}LRegion, *RegionList; 

void InitRegionList(RegionList *L);

void ListInsert(RegionList *L, int x, int y,int flag);

void SearchRegion(RegionList L, int x, int y,int flag);

void DeleteRegion(RegionList *L);

