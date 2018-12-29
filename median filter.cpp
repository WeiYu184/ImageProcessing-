// 2016302590184weiyu��ֵ�˲�.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;
void FillWhite(IplImage *pImage);
CvHistogram* CreateGrayImageHist(IplImage **ppImage);
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram);


int _tmain(int argc, _TCHAR* argv[])
{
	////////////// Raw --> BMP 
	/*FILE * fp = fopen("1024-1024.raw","rb");
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
	for ( int i = 0; i < bmp.rows; i +=10)
	{
		for ( int j = 0; j < bmp.cols; j+=10)
		{
			pBmp[i * bmp.cols + j] = 255;
		}
	}
	imwrite("raw2bmp.bmp",bmp);  // raw to tmp
	fclose(fp);
	namedWindow("raw2bmp");
	imshow("raw2bmp",bmp);       // show the image
	waitKey(0);

	///////////// Median Filtering 


	Mat bmp_filted;
	bmp_filted.create(bmp.rows,bmp.cols, CV_8UC1);
	unsigned char * pBmp_filted = bmp_filted.data; // identify the new image
	int size =7 ;
	int *pTmp = new int[size * size]; // identify the model
	for ( int i = size/2; i < bmp_filted.rows - size/2; i++)
	{
		for ( int j = size/2; j < bmp_filted.cols - size/2; j++)
		{
			// data
			int idx = 0;
			for ( int k = i - size/2; k <= i + size/2; k ++ )
			{
				for ( int l = j - size/2; l <= j + size/2; l ++ )
				{
					pTmp[idx ++] = pBmp[k * bmp.cols + l];
				}
			}
			//sorting
			for ( int k = 0; k < size *size -1;  k ++ )
			{
				int idx = k, tdata;
				for ( int l = k + 1; l < size * size; l ++)
				{
					 if( pTmp[l] < pTmp[idx]) idx = l;
				}
				if (idx != k )
				{
					tdata = pTmp[idx];
					pTmp[idx] = pTmp[k];
					pTmp[k] = tdata;
				}
			}
			//assignment
			pBmp_filted[i * bmp_filted.cols + j] = pTmp[(size*size)/2];
		}
	}

	namedWindow("bmp_filted");
	imshow("bmp_filted",bmp_filted);
	waitKey(0);

	imwrite("bmp_filted.bmp",bmp_filted);*/

	const char *pstrWindowsSrcTitle = "ԭͼ(http://blog.csdn.net/MoreWindows)";  
    const char *pstrWindowsGrayTitle = "�Ҷ�ͼ(http://blog.csdn.net/MoreWindows)";  
    const char *pstrWindowsHistTitle = "ֱ��ͼ(http://blog.csdn.net/MoreWindows)";  
    const char *pstrWindowsGrayEqualizeTitle = "�Ҷ�ͼ-���⻯��(http://blog.csdn.net/MoreWindows)";  
    const char *pstrWindowsHistEqualizeTitle = "ֱ��ͼ-���⻯��(http://blog.csdn.net/MoreWindows)";  
      
    // ���ļ��м���ԭͼ  
    IplImage *pSrcImage = cvLoadImage("ѹ��1.jpg", CV_LOAD_IMAGE_UNCHANGED);  
    IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);  
    IplImage *pGrayEqualizeImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);  
      
    // �Ҷ�ͼ  
    cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);  
    // ֱ��ͼͼ������  
    int nHistImageWidth = 255;  
    int nHistImageHeight = 150;   
    int nScale = 2;    
  
    // �Ҷ�ֱ��ͼ��ֱ��ͼͼ��  
    CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);  
    IplImage *pHistImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram);  
  
    // ���⻯  
    cvEqualizeHist(pGrayImage, pGrayEqualizeImage);  
  
    // ���⻯��ĻҶ�ֱ��ͼ��ֱ��ͼͼ��  
    CvHistogram *pcvHistogramEqualize = CreateGrayImageHist(&pGrayEqualizeImage);         
    IplImage *pHistEqualizeImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogramEqualize);  
  
	cvSaveImage( "1.jpg", pGrayEqualizeImage);
    // ��ʾ  
    //��ʾ���롭.  
    cvWaitKey(0);  
    //������Դ���롭  
    return 0;
}

void FillWhite(IplImage *pImage)  
{  
    cvRectangle(pImage, cvPoint(0, 0), cvPoint(pImage->width, pImage->height), CV_RGB(255, 255, 255), CV_FILLED);  
}  
// �����Ҷ�ͼ���ֱ��ͼ  

CvHistogram* CreateGrayImageHist(IplImage **ppImage)
{  
    int nHistSize = 256;  
    float fRange[] = {0, 255};  //�Ҷȼ��ķ�Χ    
    float *pfRanges[] = {fRange};    
    CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);  
    cvCalcHist(ppImage, pcvHistogram);  
    return pcvHistogram;  
}  
// ����ֱ��ͼ����ֱ��ͼͼ��  
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram)  
{  
    IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);  
    FillWhite(pHistImage);  
  
    //ͳ��ֱ��ͼ�е����ֱ����  
    float fMaxHistValue = 0;  
    cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);  
  
    //�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��  
    int i;  
    for(i = 0; i < nImageWidth; i++)  
    {  
        float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //����Ϊi��ֱ�����С  
        int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //Ҫ���Ƶĸ߶�  
        cvRectangle(pHistImage,  
            cvPoint(i * nScale, nImageHeight - 1),  
            cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),  
            cvScalar(i, 0, 0, 0),   
            CV_FILLED  
            );   
    }  
    return pHistImage;  
}  


