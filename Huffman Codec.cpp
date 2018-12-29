// Huffman Codec.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"math.h"
#include "stdio.h"
#include "cv.h"
#include "highgui.h"
#include "malloc.h"

using namespace cv;
using namespace std;  //使用cout输出方
/*
此程序为霍夫曼编解码
*/
using namespace std;
const int MaxValue = 59375;//初始设定的权值最大值
const int MaxBit = 21;//初始设定的最大编码位数
const int MaxN = 300;//初始设定的最大结点个数
struct HaffNode//哈夫曼树的结点结构
{
    int weight;//权值
    int flag;//标记
    int parent;//双亲结点下标
    int leftChild;//左孩子下标
    int rightChild;//右孩子下标
};
struct Code//存放哈夫曼编码的数据元素结构
{
    int bit[MaxBit];//数组
    int start;//编码的起始下标
    int weight;//字符的权值
};
void Haffman(int weight[], int n, HaffNode haffTree[])
//建立叶结点个数为n权值为weight的哈夫曼树haffTree
{
    int j, m1, m2, x1, x2;
    //哈夫曼树haffTree初始化。n个叶结点的哈夫曼树共有2n-1个结点
    for (int i = 0; i<2 * n - 1; i++)
    {
        if (i<n)
            haffTree[i].weight = weight[i];
        else
            haffTree[i].weight = 0;
        //注意这里没打else那{}，故无论是n个叶子节点还是n-1个非叶子节点都会进行下面4步的初始化
        haffTree[i].parent = 0;
        haffTree[i].flag = 0;
        haffTree[i].leftChild = -1;
        haffTree[i].rightChild = -1;
    }
    //构造哈夫曼树haffTree的n-1个非叶结点
    for (int i = 0; i<n - 1; i++)
    {
        m1 = m2 = MaxValue;//Maxvalue=10000;(就是一个相当大的数)
        x1 = x2 = 0;//x1、x2是用来保存最小的两个值在数组对应的下标
 
        for (j = 0; j<n + i; j++)//循环找出所有权重中，最小的二个值--morgan
        {
            if (haffTree[j].weight<m1&&haffTree[j].flag == 0)
            {
                m2 = m1;
                x2 = x1;
                m1 = haffTree[j].weight;
                x1 = j;
            }
            else if(haffTree[j].weight<m2&&haffTree[j].flag == 0)
            {
                m2 = haffTree[j].weight;
                x2 = j;
            }
        }
        //将找出的两棵权值最小的子树合并为一棵子树
        haffTree[x1].parent = n + i;
        haffTree[x2].parent = n + i;
        haffTree[x1].flag = 1;
        haffTree[x2].flag = 1;
        haffTree[n + i].weight = haffTree[x1].weight + haffTree[x2].weight;
        haffTree[n + i].leftChild = x1;
        haffTree[n + i].rightChild = x2;
    }
}
void HaffmanCode(HaffNode haffTree[], int n, Code haffCode[])
//由n个结点的哈夫曼树haffTree构造哈夫曼编码haffCode
{
    Code *cd = new Code;
    int child, parent;
    //求n个叶结点的哈夫曼编码
    for (int i = 0; i<n; i++)
    {
        //cd->start=n-1;//不等长编码的最后一位为n-1,
        cd->start = 0;//,----修改从0开始计数--morgan
        cd->weight = haffTree[i].weight;//取得编码对应权值的字符
        child = i;
        parent = haffTree[child].parent;
        //由叶结点向上直到根结点
        while (parent != 0)
        {
            if (haffTree[parent].leftChild == child)
                cd->bit[cd->start] = 0;//左孩子结点编码0
            else
                cd->bit[cd->start] = 1;//右孩子结点编码1
                                      //cd->start--;
            cd->start++;//改成编码自增--morgan
            child = parent;
            parent = haffTree[child].parent;
        }
        //保存叶结点的编码和不等长编码的起始位
        //for(intj=cd->start+1;j<n;j++)
        for (int j = cd->start - 1; j >= 0; j--)//重新修改编码，从根节点开始计数--morgan
            haffCode[i].bit[cd->start - j - 1] = cd->bit[j];
 
        haffCode[i].start = cd->start;
        haffCode[i].weight = cd->weight;//保存编码对应的权值
    }
}




int _tmain(int argc, _TCHAR* argv[])
{

    	Mat img;//定义原图像矩阵
	img = imread("ik_beijing_p.bmp");//读取原图像
	
	if(img.empty()) 
		return -1;//判断图像是否为空
	
	unsigned char *pImg = img.data;//获取图像指针
	int width = img.cols;
	int height = img.rows;//获取图像宽度和高度

	///////////////////第一步：得到图像每一个灰度值的频率
	int H;
	int k;
    int q;

	//灰度值频率数组
	double temp[255];
	//
	int weight[255];  
	unsigned long ICountSum;
   //灰度值总数
	int iColorNum = 256;

	//图像像素总数
	ICountSum=width*height;

	
	//初始化灰度值频率数组
   for(q = 0 ; q< iColorNum ; q++ )
	   temp[q]=0.0;

   //统计每一种灰度值有多少个像素
for ( q = 0; q < height; q++)
	{
		for ( int j = 0; j < width; j++)
		{
		
			k=pImg[q*width + j];
				temp[k]+=1;

		}
	}

	//计算各个灰度值出现的概率
for( q=0;q< iColorNum;q++)

	temp[q]/=ICountSum;


for( q=0;q< iColorNum;q++)

	printf("灰度值%d出现的概率是:%lf  \n",q,temp[q]);

	//初始化灰度值频率数组
   for(q = 0 ; q< iColorNum ; q++ )
	   weight[q]=0.0;

   //统计每一种灰度值有多少个像素
for ( q = 0; q < height; q++)
	{
		for ( int j = 0; j < width; j++)
		{
		
			k=pImg[q*width + j];
				weight[k]+=1;

		}
	}

	//计算各个灰度值出现的概率
for( q=0;q< iColorNum-100;q++)

	weight[q]/=ICountSum;





    ///////////////////第二步：根据频率确定每个灰度值所对应的霍夫曼编码


int i, j, n = 256, m = 0;
    

    HaffNode*myHaffTree = new HaffNode[2 * n - 1];
    Code*myHaffCode = new Code[n];
  
    Haffman(weight, n, myHaffTree);
    HaffmanCode(myHaffTree, n, myHaffCode);
    //输出每个叶结点的哈夫曼编码
    for (i = 0; i<n; i++)
    {
        printf( "灰度值%d" ,i);
		printf( "   霍夫曼编码是：");
        //for(j=myHaffCode[i].start+1;j<n;j++)
        for (j = 0; j<myHaffCode[i].start; j++)
		{
           printf("%d", myHaffCode[i].bit[j]);
		}

	printf("\n");
	
      
    }






	 ///////////////////第四步：图像的霍夫曼解码
	
	//输入灰度值

	printf("please input  :");
	scanf("%d",&k);

	//显示灰度值出现的概率


   
	
	   printf("灰度值%d出现的概率是:%lf  \n",k,temp[k]);



	//显示灰度值对应的霍夫曼编码
	

  
    Haffman(weight, n, myHaffTree);
    HaffmanCode(myHaffTree, n, myHaffCode);
	//输出每个叶结点的哈夫曼编码
             i=k;

        printf( "灰度值%d" ,i);
		printf( "   霍夫曼编码是：");
        //for(j=myHaffCode[i].start+1;j<n;j++)
        for (j = 0; j<myHaffCode[i].start; j++)
		{
           printf("%d", myHaffCode[i].bit[j]);
		}
	printf("\n");
	
      
    
	

    ///////////////////第三步：遍历整个图像获得图像的霍夫曼编码并存储在文件里

	FILE *fp;//文件类指针
	fp = fopen("F:\\刘子瑞的数图实习\\Huffman Codec\\Huffman Codec\\result.txt", "w");

           
for ( q = 0; q < height; q++)
	{
		for ( int j = 0; j < width; j++)
		{

			k=pImg[q*width + j];

			for (H = 0; H<myHaffCode[k].start; H++)
		  {

           fprintf(fp, "%d", myHaffCode[k].bit[H]);

		  }
		   

		}
	}

    fclose(fp);


	return 0;
}

