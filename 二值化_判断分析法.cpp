// ��ֵ��_�жϷ�����.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	////////////// Raw --> BMP 
	FILE * fp = fopen("1024-1024.raw","rb");
	unsigned char *pData = new unsigned char [1024*1024];
	fread(pData,1,1024*1024, fp);
	Mat bmp;
	bmp.create(1024,1024,CV_8UC1);
	unsigned char * pBmp = bmp.data;
	for ( int i = 0; i < bmp.rows; i ++)
	{
		for ( int j = 0; j < bmp.cols; j++)
		{
			pBmp[i * bmp.cols + j] = pData[i * bmp.cols + j];
		}
	}
	imwrite("raw2bmp.bmp",bmp);  // raw to tmp
	fclose(fp);
	namedWindow("raw2bmp");
	imshow("raw2bmp",bmp);       // show the image
	waitKey(0);

	int width = bmp.cols;
	int height = bmp.rows;

	// �Ҷ�ͳ������hist[256]:
	int hist[256];
	memset(hist, 0, sizeof(hist));

	// ͳ��bmp������С�Ҷ�ֵ��
	int mingray = 255, maxgray = 0;
	for (int i = 0; i < height; i++)
	{
		for ( int j = 0; j < width; j++)
		{
			if(pBmp[i *width +j] > maxgray)maxgray = pBmp[i *width +j];
			if(pBmp[i *width +j] < mingray)mingray = pBmp[i *width +j];
			hist[pBmp[i *width +j]]++;
		}
	} 
	printf("ԭͼ��ĻҶȷ�Χ�ǣ�[%d, %d]\n",mingray, maxgray);
	
	int IS = 0, IS1 = 0, IS2 = 0; // ���������� ��1������������2��������
	double IP = 0, IP1 = 0;   // �ܵ������غ���1������
	double m1, m2;    // ��ĻҶȾ�ֵ
	double v1 = 0, v2 = 0, classin = 0, classout = 0, max = 0; // ����v1, v2, ���ڷ����䷽��
	double ratio = 0;
	if(mingray == 0) mingray++;
	for(int k = mingray; k <= maxgray; k++)
	{
		IP += (double)k * (double)hist[k];
		IS += hist[k];
	}

	// �жϷ�������
	int k = 0, T = 0;          // TΪ�жϷ���������ֵ
	for( k = mingray; k <= maxgray; k++)
	{
			IS1 += hist[k];  // ��1��������
		if( !IS1 ) {continue; }
		IS2 = IS - IS1;  // ��2��������
		if( IS2 == 0 ) {break; }
		IP1 += (double)k * hist[k]; // ��1������
		m1 = IP1/IS1;    // ��1��ֵ
		v1 = 0, v2 = 0;
		for(int n = mingray; n <= k; n++)
			v1 += ((n - m1)*(n - m1)*hist[n]); // ��1�䷽��
		m2 = (IP - IP1)/IS2; // ��2��ֵ
		for(int m = k+1; m <= maxgray; m++)
			v2 += ((m - m2)*(m - m2)*hist[m]); // ��2�䷽��
		classin = v1 + v2; // ���ڷ���
		classout = (double)IS1 * (double)IS2 * (m1 - m2) * (m1 - m2); // ��䷽��
		if(classin != 0) ratio = classout/classin; // ��ֵ
		if(ratio > max)
		{
			max = ratio;
			T = k;
		}
	}

	// ��ֵ��:
	Mat bmp_two;
	bmp_two.create(bmp.rows,bmp.cols, CV_8UC1);
	unsigned char * pbmp_two = bmp_two.data; 
	for (int i = 0; i < height; i++)
	{
		for ( int j = 0; j < width; j++)
		{
			if( pBmp[i *width +j] < T )pbmp_two[i *width +j] = 0;
			if( pBmp[i *width +j] >= T )pbmp_two[i *width +j] = 255;
		}
	} 

	// ��������
	imwrite("��ֵ��.bmp",bmp_two);
	namedWindow("��ֵ��");
	imshow("��ֵ��",bmp_two); 
	waitKey(0);
	return 0;
}
