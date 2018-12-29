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
	int InitProcess();    //��ʼ������������ͼ���
	void Binaryzation();  //ͼ���ֵ��
	void SearchNewSeed(int x, int y, int flag,int nLeft,int nRight);//Ѱ���µ�����
	void EdgeGenerate();  //�����ֵͼ���4���ӱ߽�
	int FillLeft(int x, int y, int flag);//�������ֱ�������߽�
	int FillRight(int x, int y, int flag);//�������ֱ�������߽�
	void LineSeedFill(int x, int y, int flag);//ɨ�����������
	void Final();
	void ShowFeatures(char * filename);//�����ͨ�������ʼ�����꣬�ܳ������
public:

	Mat imgBinary;  //��ֵͼ��
	Mat imgColor;   //�ò�ͬ����ɫ��ע��ͬ�����ӳɷ�
	Mat imgGray;    //ԭͼ��
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

