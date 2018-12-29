
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;


// 函数：逆光处理
void BackLight(cv::Mat img);
// 函数：RGB to HSI
void HIS(cv::Mat img,float *Hue,float *Intensity,float *Saturation);
// 函数：HSI to RGB
void HIS2RGB(cv::Mat img, float *Hue, float *pro_Intensity, float *Saturation);
// 函数：寻找阈值，返回值为T
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
		float T = 0.0; // 亮度阈值,范围在0-1

	    static float  Hue[1024*1024];
		static float  Intensity[1024*1024]; 
		static float  Saturation[1024*1024];

		//处理后的亮度
		static float pro_Intensity[1024*1024];

		//组间方差
		double delt[256];

		//变换到HIS空间
		HIS(img, Hue, Intensity, Saturation);

		//选取合适的T
		T = Binary_T(Intensity, Height, Width);

		//亮度变换
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
			// 归一化处理，此步之后，b g r 范围为[0, 1], 相应的H I S 范围变为[0，360)，[0，1]，[0，1]
			B= b/255.0;
			G= g/255.0;
			R= r/255.0;
			//三角形正变换
			Intensity[i*Width+j]=(R+G+B)/3.0;
			if(r==b && g==r)//当r=g=b时
			{
				Hue[i*Width+j] = 0;
				Saturation[i*Width+j] = 0;
			}
			else if(b<=g && b<=r)//当b最小时
			{
				Hue[i*Width+j] = (G-B)/(3*(Intensity[i*Width+j]-B));
				Saturation[i*Width+j] = 1-(B/Intensity[i*Width+j]);
			}
			else if(r<=g && r<=b)//当r最小时
			{
				Hue[i*Width+j] = (B-R)/(3*(Intensity[i*Width+j]-R));
				Saturation[i*Width+j] = 1-(R/Intensity[i*Width+j]);
			}
			else if(g<=b && g<=r)//当g最小时
			{
				Hue[i*Width+j]=(R-G)/(3*(Intensity[i*Width+j]-G));
				Saturation[i*Width+j]=1-(G/Intensity[i*Width+j]);
			}

		}
	}

}

float Binary_T(float *Intensity, int height,int width)
{ 
	// 亮度统计
	int hist[256]; // 亮度范围：0-1，我将其分为256个亮度级进行ostu
	memset(hist, 0, sizeof(hist));

	// 统计最大和最小亮度值：
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
	printf("原图像的亮度范围是：[%f, %f]\n",minI, maxI);
	
	int IS = 0, IS1 = 0, IS2 = 0; // 像素总数， 类1像素总数，类2像素总数
	double IP = 0, IP1 = 0;   // 总的质量矩和类1质量矩
	double m1, m2;    // 类的亮度均值
	double v1 = 0, v2 = 0, classin = 0, classout = 0, max = 0; // 方差v1, v2, 类内方差，类间方差
	double ratio = 0;
	// 为便于计算，现全部转化为255倍，以整数做运算
	int Imin = int(minI*255+0.5);
	int Imax = int(maxI*255+0.5);
	if(Imin == 0) Imin++;
	for(int k = Imin; k <= Imax; k++)
	{
		IP += (double)k * (double)hist[k];
		IS += hist[k];
	}

	// 判断分析法：
	int k = 0, T = 0;          // T为判断分析法的阈值
	for( k = Imin; k <= Imax; k++)
	{
			IS1 += hist[k];  // 类1像素总数
		if( !IS1 ) {continue; }
		IS2 = IS - IS1;  // 类2像素总数
		if( IS2 == 0 ) {break; }
		IP1 += (double)k * hist[k]; // 类1质量矩
		m1 = IP1/IS1;    // 类1均值
		v1 = 0, v2 = 0;
		for(int n = Imin; n <= k; n++)
			v1 += ((n - m1)*(n - m1)*hist[n]); // 类1间方差
		m2 = (IP - IP1)/IS2; // 类2均值
		for(int m = k+1; m <= Imax; m++)
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
	printf("您的T值为%d", T);
	return (float)(T/255.0);
}

void HIS2RGB(cv::Mat img, float *Hue, float *pro_Intensity, float *Saturation)
{
	int Width = img.cols;
	int Higth = img.rows;
	unsigned char *pImg = img.data;

	Mat realImg;
	realImg.create(Higth,Width,CV_8UC3);
	unsigned char *realData = realImg.data; // 为处理后的图像分配空间

	///////////// HIS to RGB
	for(int i=0;i<Higth;i++)
	{
		for(int j=0;j<Width;j++)
		{
			float R1, G1, B1;

			int b = pImg[(i*Width+j)*3 + 0];
			int g = pImg[(i*Width+j)*3 + 1];
			int r = pImg[(i*Width+j)*3 + 2];
			
			if(r==b&&g==r)//当r=g=b时
			{
				B1=255*pro_Intensity[i*Width+j];
				G1=255*pro_Intensity[i*Width+j];
				R1=255*pro_Intensity[i*Width+j];
			}	
			else if(b<=g&&b<=r)//当b最小时
			{
				B1=255*fabs(pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				G1=255*fabs(3.0*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				R1=255*fabs(2.0*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-3*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
			}
			else if(r<=g&&r<=b)//当r最小时
			{
				B1=255*fabs(3*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				G1=255*fabs(2*pro_Intensity[i*Width+j]*Saturation[i*Width+j]+pro_Intensity[i*Width+j]-3*Hue[i*Width+j]*pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
				R1=255*fabs(pro_Intensity[i*Width+j]-pro_Intensity[i*Width+j]*Saturation[i*Width+j]);
			}
			else if(g<=r&&g<=b)//当g最小时
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
