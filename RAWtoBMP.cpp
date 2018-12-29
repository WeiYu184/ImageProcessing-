// RAWtoBMP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;  //使用cout输出方式

int _tmain(int argc, _TCHAR* argv[])
{

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

	imwrite("raw2bmp.bmp",bmp);
	fclose(fp);

	return 0;
}
