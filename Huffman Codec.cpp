// Huffman Codec.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"math.h"
#include "stdio.h"
#include "cv.h"
#include "highgui.h"
#include "malloc.h"

using namespace cv;
using namespace std;  //ʹ��cout�����
/*
�˳���Ϊ�����������
*/
using namespace std;
const int MaxValue = 59375;//��ʼ�趨��Ȩֵ���ֵ
const int MaxBit = 21;//��ʼ�趨��������λ��
const int MaxN = 300;//��ʼ�趨����������
struct HaffNode//���������Ľ��ṹ
{
    int weight;//Ȩֵ
    int flag;//���
    int parent;//˫�׽���±�
    int leftChild;//�����±�
    int rightChild;//�Һ����±�
};
struct Code//��Ź��������������Ԫ�ؽṹ
{
    int bit[MaxBit];//����
    int start;//�������ʼ�±�
    int weight;//�ַ���Ȩֵ
};
void Haffman(int weight[], int n, HaffNode haffTree[])
//����Ҷ������ΪnȨֵΪweight�Ĺ�������haffTree
{
    int j, m1, m2, x1, x2;
    //��������haffTree��ʼ����n��Ҷ���Ĺ�����������2n-1�����
    for (int i = 0; i<2 * n - 1; i++)
    {
        if (i<n)
            haffTree[i].weight = weight[i];
        else
            haffTree[i].weight = 0;
        //ע������û��else��{}����������n��Ҷ�ӽڵ㻹��n-1����Ҷ�ӽڵ㶼���������4���ĳ�ʼ��
        haffTree[i].parent = 0;
        haffTree[i].flag = 0;
        haffTree[i].leftChild = -1;
        haffTree[i].rightChild = -1;
    }
    //�����������haffTree��n-1����Ҷ���
    for (int i = 0; i<n - 1; i++)
    {
        m1 = m2 = MaxValue;//Maxvalue=10000;(����һ���൱�����)
        x1 = x2 = 0;//x1��x2������������С������ֵ�������Ӧ���±�
 
        for (j = 0; j<n + i; j++)//ѭ���ҳ�����Ȩ���У���С�Ķ���ֵ--morgan
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
        //���ҳ�������Ȩֵ��С�������ϲ�Ϊһ������
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
//��n�����Ĺ�������haffTree�������������haffCode
{
    Code *cd = new Code;
    int child, parent;
    //��n��Ҷ���Ĺ���������
    for (int i = 0; i<n; i++)
    {
        //cd->start=n-1;//���ȳ���������һλΪn-1,
        cd->start = 0;//,----�޸Ĵ�0��ʼ����--morgan
        cd->weight = haffTree[i].weight;//ȡ�ñ����ӦȨֵ���ַ�
        child = i;
        parent = haffTree[child].parent;
        //��Ҷ�������ֱ�������
        while (parent != 0)
        {
            if (haffTree[parent].leftChild == child)
                cd->bit[cd->start] = 0;//���ӽ�����0
            else
                cd->bit[cd->start] = 1;//�Һ��ӽ�����1
                                      //cd->start--;
            cd->start++;//�ĳɱ�������--morgan
            child = parent;
            parent = haffTree[child].parent;
        }
        //����Ҷ���ı���Ͳ��ȳ��������ʼλ
        //for(intj=cd->start+1;j<n;j++)
        for (int j = cd->start - 1; j >= 0; j--)//�����޸ı��룬�Ӹ��ڵ㿪ʼ����--morgan
            haffCode[i].bit[cd->start - j - 1] = cd->bit[j];
 
        haffCode[i].start = cd->start;
        haffCode[i].weight = cd->weight;//��������Ӧ��Ȩֵ
    }
}




int _tmain(int argc, _TCHAR* argv[])
{

    	Mat img;//����ԭͼ�����
	img = imread("ik_beijing_p.bmp");//��ȡԭͼ��
	
	if(img.empty()) 
		return -1;//�ж�ͼ���Ƿ�Ϊ��
	
	unsigned char *pImg = img.data;//��ȡͼ��ָ��
	int width = img.cols;
	int height = img.rows;//��ȡͼ���Ⱥ͸߶�

	///////////////////��һ�����õ�ͼ��ÿһ���Ҷ�ֵ��Ƶ��
	int H;
	int k;
    int q;

	//�Ҷ�ֵƵ������
	double temp[255];
	//
	int weight[255];  
	unsigned long ICountSum;
   //�Ҷ�ֵ����
	int iColorNum = 256;

	//ͼ����������
	ICountSum=width*height;

	
	//��ʼ���Ҷ�ֵƵ������
   for(q = 0 ; q< iColorNum ; q++ )
	   temp[q]=0.0;

   //ͳ��ÿһ�ֻҶ�ֵ�ж��ٸ�����
for ( q = 0; q < height; q++)
	{
		for ( int j = 0; j < width; j++)
		{
		
			k=pImg[q*width + j];
				temp[k]+=1;

		}
	}

	//��������Ҷ�ֵ���ֵĸ���
for( q=0;q< iColorNum;q++)

	temp[q]/=ICountSum;


for( q=0;q< iColorNum;q++)

	printf("�Ҷ�ֵ%d���ֵĸ�����:%lf  \n",q,temp[q]);

	//��ʼ���Ҷ�ֵƵ������
   for(q = 0 ; q< iColorNum ; q++ )
	   weight[q]=0.0;

   //ͳ��ÿһ�ֻҶ�ֵ�ж��ٸ�����
for ( q = 0; q < height; q++)
	{
		for ( int j = 0; j < width; j++)
		{
		
			k=pImg[q*width + j];
				weight[k]+=1;

		}
	}

	//��������Ҷ�ֵ���ֵĸ���
for( q=0;q< iColorNum-100;q++)

	weight[q]/=ICountSum;





    ///////////////////�ڶ���������Ƶ��ȷ��ÿ���Ҷ�ֵ����Ӧ�Ļ���������


int i, j, n = 256, m = 0;
    

    HaffNode*myHaffTree = new HaffNode[2 * n - 1];
    Code*myHaffCode = new Code[n];
  
    Haffman(weight, n, myHaffTree);
    HaffmanCode(myHaffTree, n, myHaffCode);
    //���ÿ��Ҷ���Ĺ���������
    for (i = 0; i<n; i++)
    {
        printf( "�Ҷ�ֵ%d" ,i);
		printf( "   �����������ǣ�");
        //for(j=myHaffCode[i].start+1;j<n;j++)
        for (j = 0; j<myHaffCode[i].start; j++)
		{
           printf("%d", myHaffCode[i].bit[j]);
		}

	printf("\n");
	
      
    }






	 ///////////////////���Ĳ���ͼ��Ļ���������
	
	//����Ҷ�ֵ

	printf("please input  :");
	scanf("%d",&k);

	//��ʾ�Ҷ�ֵ���ֵĸ���


   
	
	   printf("�Ҷ�ֵ%d���ֵĸ�����:%lf  \n",k,temp[k]);



	//��ʾ�Ҷ�ֵ��Ӧ�Ļ���������
	

  
    Haffman(weight, n, myHaffTree);
    HaffmanCode(myHaffTree, n, myHaffCode);
	//���ÿ��Ҷ���Ĺ���������
             i=k;

        printf( "�Ҷ�ֵ%d" ,i);
		printf( "   �����������ǣ�");
        //for(j=myHaffCode[i].start+1;j<n;j++)
        for (j = 0; j<myHaffCode[i].start; j++)
		{
           printf("%d", myHaffCode[i].bit[j]);
		}
	printf("\n");
	
      
    
	

    ///////////////////����������������ͼ����ͼ��Ļ��������벢�洢���ļ���

	FILE *fp;//�ļ���ָ��
	fp = fopen("F:\\���������ͼʵϰ\\Huffman Codec\\Huffman Codec\\result.txt", "w");

           
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

