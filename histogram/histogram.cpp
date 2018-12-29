#include<stdio.h>

int main(void)
{
	FILE *fp1 = fopen("lena.raw","rb");
	FILE *fp2 = fopen("result1.txt","wt"); // ���ļ�

	unsigned char pData[256*256]; // ����һά����pData����ͼ��Ҷ�ֵ
	fread(pData,sizeof(unsigned char),256*256,fp1); // ��ȡͼ��Ҷ�ֵ

	int Count[256] = {0}; // ��������count�����洢0-255ÿ���Ҷ�ֵ��Ƶ��
    float frequence[256] = {0};//�洢ÿ���Ҷ�ֵ��Ƶ��
	for(int k=0;k<256;k++) Count[k] = 0; // ��ʼ��Ϊ��

	for(int i=0;i<256;i++)
	{
		for(int j=0;j<256;j++)
		{
			Count[pData[i*256+j]]++;
		} 
	}
	for(int i=0;i<256;i++)
	{
		frequence[i] = (float)Count[i]/65536;
		fprintf(fp2,"%d,%d,%f\n",i,Count[i],frequence[i]);
	}

	fclose(fp1);
	fclose(fp2);
	return 1;
}