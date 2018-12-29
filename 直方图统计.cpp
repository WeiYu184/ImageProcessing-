// 直方图统计.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	FILE *fp1 = fopen("1024-1024.raw","rb");
	FILE *fp2 = fopen("result1.txt","wt"); // 打开文件
	unsigned char *pData = new unsigned char [1024*1024];
	fread(pData,1,1024*1024, fp1); 


	int Count[256] = {0}; // 定义数组count用来存储0-255每个灰度值的频数
    float frequence[256] = {0};//存储每个灰度值的频率
	for(int k=0;k<256;k++) Count[k] = 0; // 初始化为零

	for(int i=0;i<1024;i++)
	{
		for(int j=0;j<1024;j++)
		{
			Count[pData[i*1024+j]]++;
		} 
	}
	for(int i=0;i<256;i++)
	{
		frequence[i] = (float)Count[i]/(1.024*1.024);
		fprintf(fp2,"%d,%d,%f\n",i,Count[i],frequence[i]);
	}

	fclose(fp1);
	fclose(fp2);
	return 0;
}

