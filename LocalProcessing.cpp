// ͼ��ֲ����� byΤ�� 2016302590184

#include<stdio.h>
#include<math.h>
#include <memory.h>
#define WIDTH 256
#define HEIGHT 256

int main(void)
{
	unsigned char pData[WIDTH*HEIGHT];
	unsigned char pnewData[WIDTH*HEIGHT]; // �洢ԭͼ��ͽ��ͼ��
	memset(pData, 0, WIDTH*HEIGHT);
	memset(pnewData, 0, WIDTH*HEIGHT);

	// ��ȡͼ��
	FILE *fp1 = fopen("Lena.raw", "rb");
	fread(pData, sizeof(unsigned char), 256*256, fp1);
	fclose(fp1);

	// �������
	float temp[3][3] = {0}; // ����ģ��
	printf("���������ģ��(3*3)��\n");
	scanf("%f %f %f %f %f %f %f %f %f",&temp[0][0],&temp[0][1],&temp[0][2],&temp[1][0],&temp[1][1],&temp[1][2],&temp[2][0],&temp[2][1],&temp[2][2]);

	for(int i=1;i<HEIGHT-1;i++)
		for (int j=1;j<WIDTH-1;j++)
			pnewData[i*WIDTH+j] = abs(pData[(i-1)*WIDTH+j-1]*temp[0][0]+pData[(i-1)*WIDTH+j]*temp[0][1]+pData[(i-1)*WIDTH+j+1]*temp[0][2]+pData[i*WIDTH+j-1]*temp[1][0]+pData[i*WIDTH+j]*temp[1][1]+pData[i*WIDTH+j+1]*temp[1][2]+pData[(i+1)*WIDTH+j-1]*temp[2][0]+pData[(i+1)*WIDTH+j]*temp[2][1]+pData[(i+1)*WIDTH+j+1]*temp[2][2]);
	
	// ��Ե����
	for (int j=0; j<WIDTH; j++)
	{
		pnewData[j] = pData[j]; // i=0
		pnewData[(HEIGHT-1)*WIDTH + j] = pData[(HEIGHT-1)*WIDTH + j]; // i=HEIGHT - 1
	}
	for (int i=0; i<HEIGHT; i++)
	{
		pnewData[i*WIDTH] = pData[i*WIDTH]; // j=0
		pnewData[(i+1)*WIDTH-1] = pData[(i+1)*WIDTH-1]; // j=WIDTH - 1
	}

	// ������
	FILE *fp2 = fopen("result2.raw", "wb"); // д�뷽ʽ�򿪽���ļ�
	fwrite(pnewData, sizeof(unsigned char), WIDTH*HEIGHT, fp2);
	fclose(fp2);

	return 0;
}


