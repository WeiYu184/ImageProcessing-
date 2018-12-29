// 2016302590184weiyu3.cpp : 定义控制台应用程序的入口点。
// 图像局部处理：高通滤波和低通滤波

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

	///////////// 低通滤波
	float temp1[3][3] = {1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0};
	Mat bmp_filted1;
	bmp_filted1.create(bmp.rows,bmp.cols, CV_8UC1);
	unsigned char * pBmp_filted1 = bmp_filted1.data; // identify the new image
	// 卷积
	for(int i=1;i<height-1;i++)
	{
		for (int j=1;j<width-1;j++)
		{
			pBmp_filted1[i*width + j] = fabs(pBmp[(i-1)*width+j-1]*temp1[0][0]+pBmp[(i-1)*width+j]*temp1[0][1]+pBmp[(i-1)*width+j+1]*temp1[0][2]+pBmp[i*width+j-1]*temp1[1][0]+pBmp[i*width+j]*temp1[1][1]+pBmp[i*width+j+1]*temp1[1][2]+pBmp[(i+1)*width+j-1]*temp1[2][0]+pBmp[(i+1)*width+j]*temp1[2][1]+pBmp[(i+1)*width+j+1]*temp1[2][2]);
			if(pBmp_filted1[i*width + j] > 255) pBmp_filted1[i*width + j] = 255;
		}
	}
	
	// 边缘处理
	for (int j=0; j<width; j++)
	{
		pBmp_filted1[j] = pBmp[j]; // i = 0
		pBmp_filted1[(height-1)*width + j] = pBmp[(height-1)*width + j]; // i=HEIGHT - 1
	}
	for (int i=0; i<height; i++)
	{
		pBmp_filted1[i*width] = pBmp[i*width]; // j = 0
		pBmp_filted1[(i+1)*width-1] = pBmp[(i+1)*width-1]; // j=WIDTH - 1
	}
	namedWindow("低通滤波");
	imshow("低通滤波",bmp_filted1);
	waitKey(0);

	imwrite("低通滤波.bmp",bmp_filted1);

	///////////// 高通滤波
	float temp2[3][3] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
	Mat bmp_filted2;
	bmp_filted2.create(bmp.rows,bmp.cols, CV_8UC1);
	unsigned char * pBmp_filted2 = bmp_filted2.data; // identify the new image
	// 卷积
	for(int i=1;i<height-1;i++)
	{
		for (int j=1;j<width-1;j++)
		{
			pBmp_filted2[i*width + j] = fabs(pBmp[(i-1)*width+j-1]*temp2[0][0]+pBmp[(i-1)*width+j]*temp2[0][1]+pBmp[(i-1)*width+j+1]*temp2[0][2]+pBmp[i*width+j-1]*temp2[1][0]+pBmp[i*width+j]*temp2[1][1]+pBmp[i*width+j+1]*temp2[1][2]+pBmp[(i+1)*width+j-1]*temp2[2][0]+pBmp[(i+1)*width+j]*temp2[2][1]+pBmp[(i+1)*width+j+1]*temp2[2][2]);
			if(pBmp_filted1[i*width + j] > 255) pBmp_filted1[i*width + j] = 255;
		}
	}
	
	// 边缘处理
	for (int j=0; j<width; j++)
	{
		pBmp_filted2[j] = pBmp[j]; // i = 0
		pBmp_filted2[(height-1)*width + j] = pBmp[(height-1)*width + j]; // i=HEIGHT - 1
	}
	for (int i=0; i<height; i++)
	{
		pBmp_filted2[i*width] = pBmp[i*width]; // j = 0
		pBmp_filted2[(i+1)*width-1] = pBmp[(i+1)*width-1]; // j=WIDTH - 1
	}
	namedWindow("高通滤波");
	imshow("高通滤波",bmp_filted2);
	waitKey(0);

	imwrite("高通滤波.bmp",bmp_filted2);
	return 0;
}