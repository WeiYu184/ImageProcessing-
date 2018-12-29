#pragma once
#include"opencv2\core\core.hpp"
#include"opencv2\highgui\highgui.hpp"
#include"List.h"
#include"Stack.h"

using namespace cv;
class CFeatureExtract
{
public:
	CFeatureExtract();
	~CFeatureExtract();
	int InitProcess();    //初始化：包括，打开图像等
	void Binaryzation();  //图像二值化
	void SearchNewSeed(int x, int y, int flag,int nLeft,int nRight);//寻找新的种子
	void EdgeGenerate();  //计算二值图像的4连接边界
	int FillLeft(int x, int y, int flag);//向左填充直到遇到边界
	int FillRight(int x, int y, int flag);//向右填充直到遇到边界
	void LineSeedFill(int x, int y, int flag);//扫描线种子填充
	void Final();
	void ShowFeatures(char * filename);//输出连通区域的起始点坐标，周长，面积
public:

	Mat imgBinary;  //二值图像
	Mat imgColor;   //用不同的颜色标注不同的连接成分
	Mat imgGray;    //原图像
	Mat imgEdge;
	unsigned char* pimgB;
	unsigned char*pimgC;
	unsigned char*pimgG;
	unsigned char*pimgE;
	int nHeight;          
	int nWidth;
	RegionList LpRegion;
	LinkStack SpSeed;

};

