// 灰度线性变换.cpp : 定义控制台应用程序的入口点。
// 在raw to bmp已完成的基础上
// 实现图像的点运算：灰度线性变换

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;  //使用cout输出方式

int _tmain(int argc, _TCHAR* argv[])
{

	FILE * fp = fopen("1024-1024.raw","rb");
	unsigned char *pData = new unsigned char [1024*1024];
	fread(pData,1,1024*1024, fp); // 读取原图像
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
	waitKey(0);// 显示图像

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
	} // 统计bmp最大和最小灰度值

	printf("原图像的灰度范围是：[%d, %d]\n",mingray, maxgray);
	printf("输入线性变换后的灰度范围：注意灰度范围不应超出0-255: [");
	int nmaxgray, nmingray;
	scanf("%d,%d", &nmingray, &nmaxgray);
	printf("]");
	printf("线性变换后的灰度范围：[%d,%d]", nmingray, nmaxgray);

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
	// 进行线性变换

	imwrite("result2.bmp",newbmp);
	namedWindow("线性变换");
	imshow("线性变换",newbmp); // 生成并展示线性变换后的图像
	waitKey(0);
	return 0;
}

