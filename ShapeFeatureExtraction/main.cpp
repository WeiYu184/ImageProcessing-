// 形状特征提取.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"List.h"
#include"Stack.h"
#include"FeatureExtract.h"
#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    CFeatureExtract test1;
	test1.InitProcess();
	test1.Binaryzation();
	test1.EdgeGenerate();
	test1.Final();
	test1.ShowFeatures("Result.txt");

	return 0;
}