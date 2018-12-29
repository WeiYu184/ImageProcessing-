// ImgFusion.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace cv;
using namespace std;  //使用cout输出方式

int _tmain(int argc, _TCHAR* argv[])
{
	//读取低分辨率彩色影像
	Mat colorImg;
	colorImg = imread("data1.bmp", 1);
	if (colorImg.empty())
		return -1;
	unsigned char *pColorImg = colorImg.data;
	int color_width = colorImg.cols;
	int color_height = colorImg.rows;
	namedWindow("display");
	imshow("display",colorImg);       // show the image
	waitKey(0);

	//读取高分辨率灰度图像
	Mat grayImg;
	grayImg = imread("data2.bmp", 0);           
	if (grayImg.empty()) return -1;
	unsigned char *pGrayImg = grayImg.data;
	int gray_Width = grayImg.cols;
	int gray_Height = grayImg.rows;
	namedWindow("display");
	imshow("display",grayImg);       // show the image
	waitKey(0);

	//对高分辨率灰度图像进行边缘提取
	Mat ConvImg; // 不加权融合
	ConvImg.create(grayImg.size(),grayImg.type());
	unsigned char *pConvImg= ConvImg.data;
	Mat wConvImg; // 加权融合
	wConvImg.create(grayImg.size(),grayImg.type());
	unsigned char *pwConvImg= wConvImg.data;
    
	/////// 不加权融合
	float Template1[3*3] = { 0, -1, 0, -1, 5, -1, 0, -1, 0  };//空间高通滤波模板

	// 卷积
	int size = 3;
	for (int i = size / 2; i<gray_Height - size / 2; i++)
	{
		for (int j = size / 2; j < gray_Width - size / 2; j++)
		{
			double k = 0;   // 高通滤波结果
			double w = 0;   //用于计算权之和
			for (int m = 0; m < size; m++){
				for (int n = 0; n < size; n++)
				{
					k += pGrayImg[(i - 1 + m) * gray_Width + (j - 1 + n)] * Template1[m * size + n];
					w += Template1[m*size + n];
				}
			}
			pConvImg[i * gray_Width + j] = pGrayImg[i * gray_Width + j] - (unsigned char)(k / (w*w*1.0) + 0.5);
		}
	}

	// 边缘处理
	for (int i = 0; i < gray_Width; i++){
		pConvImg[i] = pGrayImg[i];               //第一行
		pConvImg[(gray_Height - 1)*gray_Width + i] =pGrayImg[(gray_Height - 1)*gray_Width + i]; //最后一行
		pConvImg[i*gray_Height] =  pGrayImg[i * gray_Height];                //第一列
		pConvImg[i * gray_Height + gray_Width- 1] = pGrayImg[i * gray_Height + gray_Width - 1]; //最后一列
	}//高通滤波处理完成


	//将提取的边缘信息加到彩色图像上去。
	for (int i = 0; i<color_height; i++)
	{
		for (int j = 0; j < color_width; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				pColorImg[(i*color_width + j) * 3 + k] += pConvImg[i*color_width + j];
				if (pColorImg[(i*color_width + j) * 3 + k] > 255)
				{
					pColorImg[(i*color_width + j) * 3 + k] = 255;
				}
			}
		}
	}
	namedWindow("不加权融合");
	imshow("不加权融合",colorImg);       // show the image
	imwrite("不加权融合.jpg",colorImg);
	waitKey(0);


	/////// 加权融合
	//读取低分辨率彩色影像
	Mat colorImg1;
	colorImg1 = imread("data1.bmp", 1);
	unsigned char *pColorImg1 = colorImg1.data;
	float Template[3*3] = { 0, 1.0/8.0, 0, 1.0/8.0, 1.0/2.0, 1.0/8.0, 0, 1.0/8.0, 0  };//空间低通滤波模板

	for (int i = size / 2; i<gray_Height - size / 2; i++)
	{
		for (int j = size / 2; j < gray_Width - size / 2; j++)
		{
			double k = 0;   // 低通滤波结果
			double w = 0;   //用于计算权之和
			for (int m = 0; m < size; m++){
				for (int n = 0; n < size; n++)
				{
					k += pGrayImg[(i - 1 + m) * gray_Width + (j - 1 + n)] * Template[m * size + n];
					w += Template[m*size + n];
				}
			}
			pwConvImg[i * gray_Width + j] = (unsigned char)(k / (w*w*1.0) + 0.5);
		}
	}

	//边缘处理
	for (int i = 0; i < gray_Width; i++){
		pwConvImg[i] = pGrayImg[i];               //第一行
		pwConvImg[(gray_Height - 1)*gray_Width + i] =pGrayImg[(gray_Height - 1)*gray_Width + i]; //最后一行
		pwConvImg[i*gray_Height] =  pGrayImg[i * gray_Height];                //第一列
		pwConvImg[i * gray_Height + gray_Width- 1] = pGrayImg[i * gray_Height + gray_Width - 1]; //最后一列
	}//低通滤波处理完成


	//将提取的边缘信息分波段加到彩色图像上去。
	for (int i = 0; i<color_height; i++)
	{
		for (int j = 0; j < color_width; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				pColorImg1[(i*color_width + j) * 3 + k] *= (unsigned char)((float)pGrayImg[i*color_width + j] / (float)pwConvImg[i*color_width + j]);
				if (pColorImg1[(i*color_width + j) * 3 + k] > 255)
				{
					pColorImg1[(i*color_width + j) * 3 + k] = 255;
				}
			}
		}
	}

	namedWindow("加权融合");
	imshow("加权融合",colorImg1);       // show the image
	imwrite("加权融合.jpg",colorImg1);
	waitKey(0);
	return 0;
}

