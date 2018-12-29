// 二值化_判断分析法.cpp : 定义控制台应用程序的入口点。
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

	// 灰度统计数组hist[256]:
	int hist[256];
	memset(hist, 0, sizeof(hist));

	// 统计bmp最大和最小灰度值：
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
	printf("原图像的灰度范围是：[%d, %d]\n",mingray, maxgray);
	
	int IS = 0, IS1 = 0, IS2 = 0; // 像素总数， 类1像素总数，类2像素总数
	double IP = 0, IP1 = 0;   // 总的质量矩和类1质量矩
	double m1, m2;    // 类的灰度均值
	double v1 = 0, v2 = 0, classin = 0, classout = 0, max = 0; // 方差v1, v2, 类内方差，类间方差
	double ratio = 0;
	if(mingray == 0) mingray++;
	for(int k = mingray; k <= maxgray; k++)
	{
		IP += (double)k * (double)hist[k];
		IS += hist[k];
	}

	// 判断分析法：
	int k = 0, T = 0;          // T为判断分析法的阈值
	for( k = mingray; k <= maxgray; k++)
	{
			IS1 += hist[k];  // 类1像素总数
		if( !IS1 ) {continue; }
		IS2 = IS - IS1;  // 类2像素总数
		if( IS2 == 0 ) {break; }
		IP1 += (double)k * hist[k]; // 类1质量矩
		m1 = IP1/IS1;    // 类1均值
		v1 = 0, v2 = 0;
		for(int n = mingray; n <= k; n++)
			v1 += ((n - m1)*(n - m1)*hist[n]); // 类1间方差
		m2 = (IP - IP1)/IS2; // 类2均值
		for(int m = k+1; m <= maxgray; m++)
			v2 += ((m - m2)*(m - m2)*hist[m]); // 类2间方差
		classin = v1 + v2; // 类内方差
		classout = (double)IS1 * (double)IS2 * (m1 - m2) * (m1 - m2); // 类间方差
		if(classin != 0) ratio = classout/classin; // 比值
		if(ratio > max)
		{
			max = ratio;
			T = k;
		}
	}

	// 二值化:
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

	// 输出结果：
	imwrite("二值化.bmp",bmp_two);
	namedWindow("二值化");
	imshow("二值化",bmp_two); 
	waitKey(0);
	return 0;
}
