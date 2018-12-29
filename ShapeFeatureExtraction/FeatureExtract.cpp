#include "stdafx.h"
#include "FeatureExtract.h"
#include<sstream>
using namespace std;
CFeatureExtract::CFeatureExtract()
{
}


CFeatureExtract::~CFeatureExtract()
{
}

// 函数：初始化
int CFeatureExtract::InitProcess()
{
	
	imgGray = imread("B5.jpg", 0);              
	if (imgGray.empty())
		return -1;
	pimgG = imgGray.data;
	nWidth = imgGray.cols;
	nHeight = imgGray.rows; {

	}

	imgColor.create(nHeight, nWidth, CV_8UC3);
	pimgC = imgColor.data;

	imgBinary.create(nHeight, nWidth, CV_8UC1);
	pimgB = imgBinary.data;

	imgEdge.create(nHeight, nWidth, CV_8UC1);
	pimgE = imgEdge.data;

	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			pimgC[3 * (i*nWidth + j) + 0] = 0;
			pimgC[3 * (i*nWidth + j) + 1] = 0;
			pimgC[3 * (i*nWidth + j) + 2] = 0;
		}
	}
	CreatEmptyStack(&SpSeed);
	InitRegionList(&LpRegion);


}

// 函数：二值化
void CFeatureExtract::Binaryzation()
{
	//获取灰度直方图
	int nCount[256];
	for (int k = 0; k < 256; k++)
		nCount[k] = 0;
	for (int i = 0; i < nHeight; i++)
		for (int j = 0; j < nWidth; j++)
			nCount[pimgG[i*nWidth + j]]++;


	//获取灰度区间的范围
	int min, max;
	min = max = 0;
	for (int k = 0; k < 256; k++)
		if (nCount[k] != 0)
		{
			min = k;
			break;
		}
	for (int k = 255; k >= 0; k--)
		if (nCount[k] != 0)
		{
			max = k;
			break;
		}



	//判断分析
	int T = min + 1;    //阈值，初始化为所有灰度值的最小值
	double m1, m2;  //组间平均值
	double sigma1, sigma2; //方差
	double sigma, sigmaB, m;  //组内方差，组间方差、总的均值
	int w1, w2;//两组像素的个数
	double mm1, mm2;        //EX^2
	double k, Min, T0;
	T0 = T;
	w1 = 1;
	w2 = nHeight*nWidth - 1;
	m1 = min;
	m2 = 0;
	mm1 = min*min;
	mm2 = 0;
	for (int i = T; i <= max; i++)
	{
		m2 += nCount[i] * i;
		mm2 += nCount[i] * i*i;
	}
	m = (m1 + m2) / (nHeight*nWidth);
	sigma1 = mm1 - m1*m1;
	sigma2 = mm2 - m2*m2;
	sigma = sigma1 + sigma2;
	sigmaB = (m1*w2 - m2*w1)*(m1*w2 - m2*w1);
	Min = sigmaB / sigma;
	T++;
	//以上完成初始化
	//利用 DX=EX^2-(EX)^2计算方差减少一层循环，只需加、减法

	while (T < max - 1)
	{
		m1 += (T - 1)*nCount[T - 1];
		m2 -= (T - 1)*nCount[T - 1];
		w1 += nCount[T - 1];
		w2 -= nCount[T - 1];
		mm1 += (T - 1)*(T - 1)*nCount[T - 1];
		mm2 -= (T - 1)*(T - 1)*nCount[T - 1];
		m = (m1 + m2) / (nHeight*nWidth);
		sigma1 = mm1 - m1*m1;
		sigma2 = mm2 - m2*m2;
		sigma = sigma1 + sigma2;
		sigmaB = (m1*w2 - m2*w1)*(m1*w2 - m2*w1);
		k = sigmaB / sigma;
		if (k < Min)
		{
			Min = k;
			T0 = T;
		}
		T++;
	}

	//二值化
	for (int i = 0; i < nHeight; i++)
		for (int j = 0; j < nWidth; j++)
		{
			if (pimgG[i*nWidth + j] < T0)
				pimgB[i*nWidth + j] = 0;
			else
				pimgB[i*nWidth + j] = 255;
		}


}

// 函数：边缘提取
void CFeatureExtract::EdgeGenerate()
{

	//定义卷积模板
	int size=3;
	//printf("请输入卷积模板的大小\n");
	//scanf("%d", &size);
	int *Template = new int[size*size];
	//printf("\n请输入卷积模板\n");
	//for (int i = 0; i < size*size; i++)
		//scanf("%d", &Template[i]);
	Template[1] = Template[3] = Template[4] = Template[5] = Template[7] = 1;
	Template[0] = Template[2] = Template[6] = Template[8] = 0;
	//卷积

	//边界处理：保留原来的值
	int temp;
	for (int i = 0; i < nWidth; i++)
	{
		temp = pimgB[i];
		pimgE[i] = temp;                  //第一行

		temp = pimgB[(nHeight - 1)*nWidth + i];
		pimgE[(nHeight - 1)*nWidth + i] = temp;   //最后一行

		temp = pimgB[i * nHeight];
		pimgE[i*nHeight] = temp;                 //第一列

		temp = pimgB[i * nHeight + nWidth - 1];
		pimgE[i * nHeight + nWidth - 1] = temp;   //最后一列
	}


	//中间区域
	for (int i = size / 2; i<nHeight - size / 2; i++)
		for (int j = size / 2; j < nWidth - size / 2; j++)
		{
			double k = 0;
			double w = 0;   //用于计算权之和
			for (int m = 0; m < size; m++)
				for (int n = 0; n < size; n++)
				{
					k += pimgB[(i - 1 + m) * nWidth + (j - 1 + n)] * Template[m * size + n];
					w += Template[m*size + n];;
				}
			pimgE[i*nWidth + j] = unsigned char(k / 5);
			if (pimgB[i*nWidth + j] == 255 && pimgE[i*nWidth + j] != 255)
				pimgE[i*nWidth + j] = 255;
			else
				pimgE[i*nWidth + j] = 0;
		}
}

// 函数：种子填充法：寻找下一填充区域
void CFeatureExtract::SearchNewSeed(int x, int y, int flag,int nLeft,int nRight)//包括左右端点
{
	for(int j=nLeft;j<nRight;j++)
		if(pimgB[x*nWidth+j]==255&&pimgB[x*nWidth+j+1]==0&&pimgC[(x*nWidth + j) * 3]==0)
			PushStack(&SpSeed, x, j);
	if(pimgB[x*nWidth + nRight] == 255&& pimgC[(x*nWidth + nRight) * 3] == 0)
		PushStack(&SpSeed, x, nRight);
}

// 函数：种子填充法：向左填充
int CFeatureExtract::FillLeft(int X0, int Y0, int flag)
{
	int nCount=0;
	for (int j = Y0; j >= 0; j--)
	{
		
		if (pimgB[X0* nWidth + j] == 255 && pimgC[(X0*nWidth + j) * 3 + 0] == 0)  //连接成分内部且未被标记
		{
			nCount++;
			pimgC[(X0*nWidth + j) * 3 + 0] = flag % 255 + 1;
			pimgC[(X0*nWidth + j) * 3 + 1] = (2 * flag) % 255 + 1;
			pimgC[(X0*nWidth + j) * 3 + 2] = (3 * flag) % 255 + 1;

			LpRegion->next->are++;        //如果是连通区域的像素，面积+1
			if (pimgE[X0* nWidth + j] == 255)//如果是边界，则周长+1
				LpRegion->next->perimeter++;
		
		}
		else
			break;

	}
	return nCount-1;
}

// 函数：种子填充法：向右填充
int CFeatureExtract::FillRight(int X0, int Y0, int flag)
{
	int nCount = 0;
	for (int j = Y0; j < nWidth; j++)
	{

		if (pimgB[X0* nWidth + j] == 255 && pimgC[(X0*nWidth + j) * 3 + 0] == 0)
		{
			nCount++;
			pimgC[(X0*nWidth + j) * 3 + 0] = flag % 255;
			pimgC[(X0*nWidth + j) * 3 + 1] = (2 * flag) % 255;
			pimgC[(X0*nWidth + j) * 3 + 2] = (3 * flag) % 255;
			LpRegion->next->are++;        //如果是连通区域的像素，面积+1
			if (pimgE[X0* nWidth + j] == 255)//如果是边界，则周长+1
				LpRegion->next->perimeter++;
		}
		else
			break;
	}
	return nCount;
}

// 函数：种子填充法：扫描线种子填充
void CFeatureExtract::LineSeedFill(int x, int y, int flag)
{


	PushStack(&SpSeed, x, y);//种子入栈；
	while (!IsEmpty(SpSeed))
	{
		int X0, Y0;
		int nCount;    //向左、向右填充的数量；
		int nLeft, nRight; //最左端、最右端坐标；
		PopStack(&SpSeed,&X0, &Y0);
		nCount = FillLeft(X0, Y0, flag);
		nLeft = Y0 - nCount;
		nCount = FillRight(X0, Y0+1, flag);
		nRight = Y0 + nCount;

		//扫描相邻两条扫描线
		if(X0>0)
			SearchNewSeed(X0-1, Y0, flag,nLeft,nRight);
		if(X0<nHeight-1)
			SearchNewSeed(X0+1, Y0, flag,nLeft,nRight);
	}
}

// 函数：种子填充法
void CFeatureExtract::Final()
{
	int flag = 1;

	for (int i=0;i<nHeight;i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			if (pimgC[(i*nWidth + j) * 3 != 0]||pimgB[i*nWidth+j]==0)                      //已经被标记了或者是黑色的
				continue;
			else
			{
				ListInsert(&LpRegion, i, j, flag);
				LineSeedFill(i, j, flag);
				flag++;
			}
		}
	}

	
}

// 函数：输出结果和显示图像
void CFeatureExtract::ShowFeatures(char * filename)
{
	FILE *fp;
	fp = fopen(filename, "w");
	LRegion *p;
	p = LpRegion->next;
	char s[15];
	for (; p != NULL; p = p->next)
		if (p->are > 1000)
		{
			fprintf(fp, "%d %d %d %d %d \n", p->x, p->y, p->flag, p->are, p->perimeter);
			sprintf(s, "%d,%d", p->are,p->perimeter);
			int length = sqrt((double)p->are);
			Rect r((p->y-length/1.5),p->x,length,length);
			rectangle(imgColor, r, Scalar(0,0,255),3);
			putText(imgColor, s, cvPoint(p->y,p->x), cv::FONT_HERSHEY_SIMPLEX,0.7,Scalar(0,255,255),1,8);
		}
	imwrite("Final.bmp", imgColor);
	namedWindow("display3");
	imshow("display3", imgColor);
	waitKey(0);
	fclose(fp);
}