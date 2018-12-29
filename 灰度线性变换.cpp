// �Ҷ����Ա任.cpp : �������̨Ӧ�ó������ڵ㡣
// ��raw to bmp����ɵĻ�����
// ʵ��ͼ��ĵ����㣺�Ҷ����Ա任

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;  //ʹ��cout�����ʽ

int _tmain(int argc, _TCHAR* argv[])
{

	FILE * fp = fopen("1024-1024.raw","rb");
	unsigned char *pData = new unsigned char [1024*1024];
	fread(pData,1,1024*1024, fp); // ��ȡԭͼ��
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

	imwrite("raw2bmp.bmp",bmp);
	fclose(fp); // raw to bmp
	namedWindow("display");
	imshow("display",bmp);
	waitKey(0);// ��ʾͼ��

	int width = bmp.cols;
	int height = bmp.rows;
	int mingray = 0, maxgray = 0;
	for (int i = 0; i < height; i++)
	{
		for ( int j = 0; j < width; j++)
		{
			if(pBmp[i *width +j] > maxgray)maxgray = pBmp[i *width +j];
			if(pBmp[i *width +j] < mingray)mingray = pBmp[i *width +j];
		}
	} // ͳ��bmp������С�Ҷ�ֵ

	printf("ԭͼ��ĻҶȷ�Χ�ǣ�[%d, %d]\n",mingray, maxgray);
	printf("�������Ա任��ĻҶȷ�Χ��ע��Ҷȷ�Χ��Ӧ����0-255: [");
	int nmaxgray, nmingray;
	scanf("%d,%d", &nmingray, &nmaxgray);
	printf("]");
	printf("���Ա任��ĻҶȷ�Χ��[%d,%d]", nmingray, nmaxgray);

	float k;
	k = (float)(nmaxgray - nmingray)/(float)( maxgray - mingray); // k = (b'-a')/(b-a)
	
	Mat newbmp;
	newbmp.create(1024,1024,CV_8UC1);
	unsigned char * pnbmp = newbmp.data;
	for (int i = 0; i < height; i++)
	{
		for ( int j = 0; j < width; j++)
		{
			pnbmp[i *width +j] = nmingray + k * (pBmp[i *width +j] - mingray);

		}
	}
	// �������Ա任

	imwrite("result2.bmp",newbmp);
	namedWindow("���Ա任");
	imshow("���Ա任",newbmp); // ���ɲ�չʾ���Ա任���ͼ��
	waitKey(0);
	return 0;
}

