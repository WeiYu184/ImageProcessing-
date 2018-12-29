#include<stdio.h>

int main(void)
{
	FILE *fp1 = fopen("lena.raw","rb");
	FILE *fp2 = fopen("result1.txt","wt"); // 打开文件

	unsigned char pData[256*256]; // 创建一维数组pData储存图像灰度值
	fread(pData,sizeof(unsigned char),256*256,fp1); // 读取图像灰度值

	int Count[256] = {0}; // 定义数组count用来存储0-255每个灰度值的频数
    float frequence[256] = {0};//存储每个灰度值的频率
	for(int k=0;k<256;k++) Count[k] = 0; // 初始化为零

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