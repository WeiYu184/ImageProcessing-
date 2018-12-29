// 2016302590184weiyu伪彩色增强.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

// 结构体：颜色查询表
struct tRGB
{
	int R, G, B;
}; 
struct tRGB ctable[256] = {(0,0,0)};

int _tmain(int argc, _TCHAR* argv[])
{
	// 生成颜色查询表：
	for(int i = 0; i <= 15; i++){ ctable[i].R = 8; ctable[i].G = 8; ctable[i].B = 255;}
	for(int i = 16; i <= 31; i++){ ctable[i].R = 16; ctable[i].G = 48; ctable[i].B = 254;}
	for(int i = 32; i <= 47; i++){ ctable[i].R = 32; ctable[i].G = 112; ctable[i].B = 252;}
	for(int i = 48; i <= 63; i++){ ctable[i].R = 64; ctable[i].G = 160; ctable[i].B = 248;}
	for(int i = 64; i <= 79; i++){ ctable[i].R = 255; ctable[i].G = 8; ctable[i].B = 1;}
	for(int i = 80; i <= 95; i++){ ctable[i].R = 128; ctable[i].G = 192; ctable[i].B = 240;}
	for(int i = 96; i <= 111; i++){ ctable[i].R = 160; ctable[i].G = 224; ctable[i].B = 224;}
	for(int i = 112; i <= 127; i++){ ctable[i].R = 192; ctable[i].G = 248; ctable[i].B = 192;}
	for(int i = 128; i <= 143; i++){ ctable[i].R = 224; ctable[i].G = 255; ctable[i].B = 160;}
	for(int i = 144; i <= 159; i++){ ctable[i].R = 240; ctable[i].G = 255; ctable[i].B = 128;}
	for(int i = 160; i <= 175; i++){ ctable[i].R = 245; ctable[i].G = 248; ctable[i].B = 64;}
	for(int i = 176; i <= 191; i++){ ctable[i].R = 253; ctable[i].G = 224; ctable[i].B = 32;}
	for(int i = 192; i <= 207; i++){ ctable[i].R = 254; ctable[i].G = 192; ctable[i].B = 16;}
	for(int i = 208; i <= 223; i++){ ctable[i].R = 255; ctable[i].G = 160; ctable[i].B = 8;}
	for(int i = 224; i <= 239; i++){ ctable[i].R = 255; ctable[i].G = 112; ctable[i].B = 4;}
	for(int i = 240; i <= 255; i++){ ctable[i].R = 255; ctable[i].G = 48; ctable[i].B = 2;}
	/*for(int i = 0; i < 128; i++)
	{
		ctable[i].R = i;           // red = gray
		ctable[i].G = 2*i;         // green = 2 * red
		ctable[i].B = 255 - i;     // blue = 255 - gray
	}
	for(int i = 0; i > 128 && i < 256; i++)
	{
		ctable[i].R = i;            // red = gray
		ctable[i].G = 2*(255 - i);  // green = 2 * blue
		ctable[i].B = 255 - i;      //blue = 255 - gray
	}*/

	// 打开灰度图像：
	Mat bmp;
	bmp = imread("ik_beijing_p.bmp", 0);
	unsigned char *pBmp = bmp.data;
	namedWindow("灰度图像");
	imshow("灰度图像",bmp);       // show the image
	waitKey(0);

	int width = bmp.cols;
	int height = bmp.rows;

	Mat color1(height, width, CV_8UC3);//构造RGB图像
	
	for (int i = 0; i < height; i++)
	{
		for ( int j = 0; j < width; j++)
		{
			Vec3b pixel;
			pixel[0] = ctable[pBmp[i*width + j]].B;     // blue
			pixel[1] = ctable[pBmp[i*width + j]].G;     // green
			pixel[2] = ctable[pBmp[i*width + j]].R;     // red
			color1.at<Vec3b>(i,j) = pixel;
		}
	}
	namedWindow("灰度分割法");
	imshow("灰度分割法",color1);      
	imwrite("灰度分割法.bmp",color1);
	waitKey(0);


	// 空间域变换彩色合成：
	Mat color2(height, width, CV_8UC3);//构造RGB图像
	
	for (int i = 0; i < height; i++)
	{
		for ( int j = 0; j < width; j++)
		{
			Vec3b pixel;
			if(pBmp[i*width + j] <= 128)
			{
				pixel[0] = 255 - pBmp[i*width + j]; // blue
				pixel[1] = 2*pBmp[i*width + j];     // green
				pixel[2] = i;                       // red
			}
			else
			{
				pixel[0] = 255 - pBmp[i*width + j];      // blue
				pixel[1] = 2*(255 - pBmp[i*width + j]);  // green
				pixel[2] = i;                            // red
			}
			color2.at<Vec3b>(i,j) = pixel;
		}
	}
	namedWindow("空间域变换彩色合成法");
	imshow("空间域变换彩色合成法",color2);      
	imwrite("空间域变换彩色合成法.bmp",color2);
	waitKey(0);

	// 频率域变换彩色合成：

	// 低通滤波器
	// 带通滤波器
	// 高通滤波器
	return 0;
}

