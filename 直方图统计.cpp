// ֱ��ͼͳ��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	FILE *fp1 = fopen("1024-1024.raw","rb");
	FILE *fp2 = fopen("result1.txt","wt"); // ���ļ�
	unsigned char *pData = new unsigned char [1024*1024];
	fread(pData,1,1024*1024, fp1); 


	int Count[256] = {0}; // ��������count�����洢0-255ÿ���Ҷ�ֵ��Ƶ��
    float frequence[256] = {0};//�洢ÿ���Ҷ�ֵ��Ƶ��
	for(int k=0;k<256;k++) Count[k] = 0; // ��ʼ��Ϊ��

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

