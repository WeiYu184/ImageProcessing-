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

// ��������ʼ��
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

// ��������ֵ��
void CFeatureExtract::Binaryzation()
{
	//��ȡ�Ҷ�ֱ��ͼ
	int nCount[256];
	for (int k = 0; k < 256; k++)
		nCount[k] = 0;
	for (int i = 0; i < nHeight; i++)
		for (int j = 0; j < nWidth; j++)
			nCount[pimgG[i*nWidth + j]]++;


	//��ȡ�Ҷ�����ķ�Χ
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



	//�жϷ���
	int T = min + 1;    //��ֵ����ʼ��Ϊ���лҶ�ֵ����Сֵ
	double m1, m2;  //���ƽ��ֵ
	double sigma1, sigma2; //����
	double sigma, sigmaB, m;  //���ڷ����䷽��ܵľ�ֵ
	int w1, w2;//�������صĸ���
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
	//������ɳ�ʼ��
	//���� DX=EX^2-(EX)^2���㷽�����һ��ѭ����ֻ��ӡ�����

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

	//��ֵ��
	for (int i = 0; i < nHeight; i++)
		for (int j = 0; j < nWidth; j++)
		{
			if (pimgG[i*nWidth + j] < T0)
				pimgB[i*nWidth + j] = 0;
			else
				pimgB[i*nWidth + j] = 255;
		}


}

// ��������Ե��ȡ
void CFeatureExtract::EdgeGenerate()
{

	//������ģ��
	int size=3;
	//printf("��������ģ��Ĵ�С\n");
	//scanf("%d", &size);
	int *Template = new int[size*size];
	//printf("\n��������ģ��\n");
	//for (int i = 0; i < size*size; i++)
		//scanf("%d", &Template[i]);
	Template[1] = Template[3] = Template[4] = Template[5] = Template[7] = 1;
	Template[0] = Template[2] = Template[6] = Template[8] = 0;
	//���

	//�߽紦������ԭ����ֵ
	int temp;
	for (int i = 0; i < nWidth; i++)
	{
		temp = pimgB[i];
		pimgE[i] = temp;                  //��һ��

		temp = pimgB[(nHeight - 1)*nWidth + i];
		pimgE[(nHeight - 1)*nWidth + i] = temp;   //���һ��

		temp = pimgB[i * nHeight];
		pimgE[i*nHeight] = temp;                 //��һ��

		temp = pimgB[i * nHeight + nWidth - 1];
		pimgE[i * nHeight + nWidth - 1] = temp;   //���һ��
	}


	//�м�����
	for (int i = size / 2; i<nHeight - size / 2; i++)
		for (int j = size / 2; j < nWidth - size / 2; j++)
		{
			double k = 0;
			double w = 0;   //���ڼ���Ȩ֮��
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

// ������������䷨��Ѱ����һ�������
void CFeatureExtract::SearchNewSeed(int x, int y, int flag,int nLeft,int nRight)//�������Ҷ˵�
{
	for(int j=nLeft;j<nRight;j++)
		if(pimgB[x*nWidth+j]==255&&pimgB[x*nWidth+j+1]==0&&pimgC[(x*nWidth + j) * 3]==0)
			PushStack(&SpSeed, x, j);
	if(pimgB[x*nWidth + nRight] == 255&& pimgC[(x*nWidth + nRight) * 3] == 0)
		PushStack(&SpSeed, x, nRight);
}

// ������������䷨���������
int CFeatureExtract::FillLeft(int X0, int Y0, int flag)
{
	int nCount=0;
	for (int j = Y0; j >= 0; j--)
	{
		
		if (pimgB[X0* nWidth + j] == 255 && pimgC[(X0*nWidth + j) * 3 + 0] == 0)  //���ӳɷ��ڲ���δ�����
		{
			nCount++;
			pimgC[(X0*nWidth + j) * 3 + 0] = flag % 255 + 1;
			pimgC[(X0*nWidth + j) * 3 + 1] = (2 * flag) % 255 + 1;
			pimgC[(X0*nWidth + j) * 3 + 2] = (3 * flag) % 255 + 1;

			LpRegion->next->are++;        //�������ͨ��������أ����+1
			if (pimgE[X0* nWidth + j] == 255)//����Ǳ߽磬���ܳ�+1
				LpRegion->next->perimeter++;
		
		}
		else
			break;

	}
	return nCount-1;
}

// ������������䷨���������
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
			LpRegion->next->are++;        //�������ͨ��������أ����+1
			if (pimgE[X0* nWidth + j] == 255)//����Ǳ߽磬���ܳ�+1
				LpRegion->next->perimeter++;
		}
		else
			break;
	}
	return nCount;
}

// ������������䷨��ɨ�����������
void CFeatureExtract::LineSeedFill(int x, int y, int flag)
{


	PushStack(&SpSeed, x, y);//������ջ��
	while (!IsEmpty(SpSeed))
	{
		int X0, Y0;
		int nCount;    //������������������
		int nLeft, nRight; //����ˡ����Ҷ����ꣻ
		PopStack(&SpSeed,&X0, &Y0);
		nCount = FillLeft(X0, Y0, flag);
		nLeft = Y0 - nCount;
		nCount = FillRight(X0, Y0+1, flag);
		nRight = Y0 + nCount;

		//ɨ����������ɨ����
		if(X0>0)
			SearchNewSeed(X0-1, Y0, flag,nLeft,nRight);
		if(X0<nHeight-1)
			SearchNewSeed(X0+1, Y0, flag,nLeft,nRight);
	}
}

// ������������䷨
void CFeatureExtract::Final()
{
	int flag = 1;

	for (int i=0;i<nHeight;i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			if (pimgC[(i*nWidth + j) * 3 != 0]||pimgB[i*nWidth+j]==0)                      //�Ѿ�������˻����Ǻ�ɫ��
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

// ����������������ʾͼ��
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