
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;


// ��������⴦��
void BackLight(cv::Mat img);
// ������RGB to HSI
void HIS(cv::Mat img,float *Hue,float *Intensity,float *Saturation);
// ������HSI to RGB
void HIS2RGB(cv::Mat img, float *Hue, float *pro_Intensity, float *Saturation);
// ������Ѱ����ֵ������ֵΪT
float Binary_T(float *Intensity, int height,int width); 

int _tmain(int argc, _TCHAR* argv[])
{
	Mat img = imread("original.bmp");
	if(img.empty())
		return -1;
  	BackLight(img);
	return 0;
}

void BackLight(cv::Mat img)
{
		int Width = img.cols;
		int Height = img.rows;
		float T = 0.0; // ������ֵ,��Χ��0-1

	    static float  Hue[1024*1024];
		static float  Intensity[1024*1024]; 
		static float  Saturation[1024*1024];

		//����������
		static float pro_Intensity[1024*1024];

		//��䷽��
		double delt[256];

		//�任��HIS�ռ�
		HIS(img, Hue, Intensity, Saturation);

		//ѡȡ���ʵ�T
		T = Binary_T(Intensity, Height, Width);

		//���ȱ任
		int num_C1 = 0;
		for(int i =0;i < Height;i++)
		 {
				for(int j=0;j<Width;j++)
				{
					if(Intensity[i*Width + j] <= T)
						num_C1++;
				}
		 }
		float A = 0;
		float k = 0;
		printf("Please input the k you want(k = 10)\n");
		scanf("%f",&k);
		A = k*sqrt(((float)num_C1)/((float)(Width*Height-num_C1)));

		float D1 = A;
		int Base = 0;

	    float C1 = 1/log(1+D1);
		for(int i=0;i<Height;i++)
		{
			for(int j=0;j<Width;j++)
			{	
				if(Intensity[i*Width + j] <= T)
					pro_Intensity[i*Width + j] = fabs(C1*log(D1*Intensity[i*Width + j]+1));
				if(Intensity[i*Width + j] > T)
				{
					float D2 = (T*A-T)/((1-T)*Intensity[i*Width + j]) - (T*A-1)/(1-T);
					float C2 = 1/log(D2+1);
					pro_Intensity[i*Width + j] = fabs(C2*log(D2*Intensity[i*Width + j]+1));
				}
				if(pro_Intensity[i*Width + j]<=0)pro_Intensity[i*Width + j] = 1.0/255.0;
				if(pro_Intensity[i*Width + j]>1)pro_Intensity[i*Width + j] = 1;
			}
		}
	 HIS2RGB(img, Hue, pro_Intensity, Saturation);
	 return;
}

void HIS(cv::Mat img,float *Hue,float *Intensity,float *Saturation)
{
	unsigned char *pImg = img.data;
	int Height = img.rows;
	int Width = img.cols;
	int r,g,b;
	float R, G, B;
	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{
			b=pImg[(i*Width+j)*3+0];
			g=pImg[(i*Width+j)*3+1];
			r=pImg[(i*Width+j)*3+2]; 
			// ��һ�������˲�֮��b g r ��ΧΪ[0, 1], ��Ӧ��H I S ��Χ��Ϊ[0��360)��[0��1]��[0��1]
			B= b/255.0;
			G= g/255.0;
			R= r/255.0;
			//���������任
			Intensity[i*Width+j]=(R+G+B)/3.0;
			if(r==b && g==r)//��r=g=bʱ
			{
				Hue[i*Width+j] = 0;
				Saturation[i*Width+j] = 0;
			}
			else if(b<=g && b<=r)//��b��Сʱ
			{
				Hue[i*Width+j] = (G-B)/(3*(Intensity[i*Width+j]-B));
				Saturation[i*Width+j] = 1-(B/Intensity[i*Width+j]);
			}
			else if(r<=g && r<=b)//��r��Сʱ
			{
				Hue[i*Width+j] = (B-R)/(3*(Intensity[i*Width+j]-R));
				Saturation[i*Width+j] = 1-(R/Intensity[i*Width+j]);
			}
			else if(g<=b && g<=r)//��g��Сʱ
			{
				Hue[i*Width+j]=(R-G)/(3*(Intensity[i*Width+j]-G));
				Saturation[i*Width+j]=1-(G/Intensity[i*Width+j]);
			}

		}
	}

}

float Binary_T(float *Intensity, int height,int width)
{ 
	// ����ͳ��
	int hist[256]; // ���ȷ�Χ��0-1���ҽ����Ϊ256�����ȼ�����ostu
	memset(hist, 0, sizeof(hist));

	// ͳ��������С����ֵ��
	float minI = 1, maxI = 0;
	for (int i = 0; i < height; i++)
	{
		for ( int j = 0; j < width; j++)
		{
			if(Intensity[i *width +j] > maxI)maxI = Intensity[i *width +j];
			if(Intensity[i *width +j] < minI)minI = Intensity[i *width +j];
			hist[(int)(255*Intensity[i *width +j] + 0.5)]++;
		}
	} 
	printf("ԭͼ������ȷ�Χ�ǣ�[%f, %f]\n",minI, maxI);
	
	int IS = 0, IS1 = 0, IS2 = 0; // ���������� ��1������������2��������
	double IP = 0, IP1 = 0;   // �ܵ������غ���1������
	double m1, m2;    // ������Ⱦ�ֵ
	double v1 = 0, v2 = 0, classin = 0, classout = 0, max = 0; // ����v1, v2, ���ڷ����䷽��
	double ratio = 0;
	// Ϊ���ڼ��㣬��ȫ��ת��Ϊ255����������������
	int Imin = int(minI*255+0.5);
	int Imax = int(maxI*255+0.5);
	if(Imin == 0) Imin++;
	for(int k = Imin; k <= Imax; k++)
	{
		IP += (double)k * (double)hist[k];
		IS += hist[k];
	}

	// �жϷ�������
	int k = 0, T = 0;          // TΪ�жϷ���������ֵ
	for( k = Imin; k <= Imax; k++)
	{
			IS1 += hist[k];  // ��1��������
		if( !IS1 ) {continue; }
		IS2 = IS - IS1;  // ��2��������
		if( IS2 == 0 ) {break; }
		IP1 += (double)k * hist[k]; // ��1������
		m1 = IP1/IS1;    // ��1��ֵ
		v1 = 0, v2 = 0;
		for(int n = Imin; n <= k; n++)
			v1 += ((n - m1)*(n - m1)*hist[n]); // ��1�䷽��
		m2 = (IP - IP1)/IS2; // ��2��ֵ
		for(int m = k+1; m <= Imax; m++)
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
	printf("����TֵΪ%d", T);
	return (float)(T/255.0);
}

void HIS2RGB(cv::Mat img, float *Hue, float *pro_Intensity, float *Saturation)
{
	int Width = img.cols;
	int Higth = img.rows;
	unsigned char *pImg = img.data;

	Mat realImg;
	realImg.create(Higth,Width,CV_8UC3);
	unsigned char *realData = realImg.data; // Ϊ������ͼ�����ռ�

	///////////// HIS to RGB
	for(int i=0;i<Higth;i++)
	{
		for(int j=0;j<Width;j++)
		{
			float R1, G1, B1;

			int b = pImg[(i*Width+j)*3 + 0];
			int g = pImg[(i*Width+j)*3 + 1];
			int r = pImg[(i*Width+j)*3 + 2];
			
			if(r==b&&g==r)//��r=g=bʱ
			{
				B1=255*pro_Intensity[i*Width+j];
				G1=255*pro_Intensity[i*Width+j];
				R1=255*pro_Intensity[i*Width+j];
			}	
			else if(b<=g&&b<=r)//��b��Сʱ
			{
				B1=255*fabs(pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				G1=255*fabs(3.0*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				R1=255*fabs(2.0*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-3*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
			}
			else if(r<=g&&r<=b)//��r��Сʱ
			{
				B1=255*fabs(3*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				G1=255*fabs(2*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-3*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				R1=255*fabs(pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
			}
			else if(g<=r&&g<=b)//��g��Сʱ
			{
				B1=255*fabs(2*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-3*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				G1=255*fabs(pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				R1=255*fabs(3*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
			}
			if(B1>255)B1=255;
			if(G1>255)G1=255;
			if(R1>255)R1=255;
			realData[(i*Width+j)*3+0] = B1;
			realData[(i*Width+j)*3+1] = G1;
			realData[(i*Width+j)*3+2] = R1;
		}
	}

	///////////// print the result
	imwrite("result.bmp",realImg);
	namedWindow("display");
	imshow("display",realImg);
	waitKey();
	return;
}
