// ImgFusion.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace cv;
using namespace std;  //ʹ��cout�����ʽ

int _tmain(int argc, _TCHAR* argv[])
{
	//��ȡ�ͷֱ��ʲ�ɫӰ��
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

	//��ȡ�߷ֱ��ʻҶ�ͼ��
	Mat grayImg;
	grayImg = imread("data2.bmp", 0);           
	if (grayImg.empty()) return -1;
	unsigned char *pGrayImg = grayImg.data;
	int gray_Width = grayImg.cols;
	int gray_Height = grayImg.rows;
	namedWindow("display");
	imshow("display",grayImg);       // show the image
	waitKey(0);

	//�Ը߷ֱ��ʻҶ�ͼ����б�Ե��ȡ
	Mat ConvImg; // ����Ȩ�ں�
	ConvImg.create(grayImg.size(),grayImg.type());
	unsigned char *pConvImg= ConvImg.data;
	Mat wConvImg; // ��Ȩ�ں�
	wConvImg.create(grayImg.size(),grayImg.type());
	unsigned char *pwConvImg= wConvImg.data;
    
	/////// ����Ȩ�ں�
	float Template1[3*3] = { 0, -1, 0, -1, 5, -1, 0, -1, 0  };//�ռ��ͨ�˲�ģ��

	// ���
	int size = 3;
	for (int i = size / 2; i<gray_Height - size / 2; i++)
	{
		for (int j = size / 2; j < gray_Width - size / 2; j++)
		{
			double k = 0;   // ��ͨ�˲����
			double w = 0;   //���ڼ���Ȩ֮��
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

	// ��Ե����
	for (int i = 0; i < gray_Width; i++){
		pConvImg[i] = pGrayImg[i];               //��һ��
		pConvImg[(gray_Height - 1)*gray_Width + i] =pGrayImg[(gray_Height - 1)*gray_Width + i]; //���һ��
		pConvImg[i*gray_Height] =  pGrayImg[i * gray_Height];                //��һ��
		pConvImg[i * gray_Height + gray_Width- 1] = pGrayImg[i * gray_Height + gray_Width - 1]; //���һ��
	}//��ͨ�˲��������


	//����ȡ�ı�Ե��Ϣ�ӵ���ɫͼ����ȥ��
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
	namedWindow("����Ȩ�ں�");
	imshow("����Ȩ�ں�",colorImg);       // show the image
	imwrite("����Ȩ�ں�.jpg",colorImg);
	waitKey(0);


	/////// ��Ȩ�ں�
	//��ȡ�ͷֱ��ʲ�ɫӰ��
	Mat colorImg1;
	colorImg1 = imread("data1.bmp", 1);
	unsigned char *pColorImg1 = colorImg1.data;
	float Template[3*3] = { 0, 1.0/8.0, 0, 1.0/8.0, 1.0/2.0, 1.0/8.0, 0, 1.0/8.0, 0  };//�ռ��ͨ�˲�ģ��

	for (int i = size / 2; i<gray_Height - size / 2; i++)
	{
		for (int j = size / 2; j < gray_Width - size / 2; j++)
		{
			double k = 0;   // ��ͨ�˲����
			double w = 0;   //���ڼ���Ȩ֮��
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

	//��Ե����
	for (int i = 0; i < gray_Width; i++){
		pwConvImg[i] = pGrayImg[i];               //��һ��
		pwConvImg[(gray_Height - 1)*gray_Width + i] =pGrayImg[(gray_Height - 1)*gray_Width + i]; //���һ��
		pwConvImg[i*gray_Height] =  pGrayImg[i * gray_Height];                //��һ��
		pwConvImg[i * gray_Height + gray_Width- 1] = pGrayImg[i * gray_Height + gray_Width - 1]; //���һ��
	}//��ͨ�˲��������


	//����ȡ�ı�Ե��Ϣ�ֲ��μӵ���ɫͼ����ȥ��
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

	namedWindow("��Ȩ�ں�");
	imshow("��Ȩ�ں�",colorImg1);       // show the image
	imwrite("��Ȩ�ں�.jpg",colorImg1);
	waitKey(0);
	return 0;
}

