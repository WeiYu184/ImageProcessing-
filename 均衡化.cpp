#include<stdio.h>
#include<math.h>

int main(void)
{
	FILE* fp1 = fopen("result1.txt","rt");
	int nr[256] = {0};
	float pr[256] = {0};
	for(int i=0; i<256; i++)fscanf(fp1,"%d, %d, %f\n", &i,&nr[i], &pr[i]);
	float skj[256] = {0};
	float skb[256] = {0};
	for(int i = 0; i<256; i++)
	{
		for(int j=0; j<=i; j++)skj[i] += pr[j];
	}// 计算sk计
	for(int i = 0; i < 256; i++)
	{
		for(int j=0; j <= 256; j++)
		{
			if(abs(skj[i]-(float)j/256) < 1/512)skb[i] = (float)j/256;
		}
	}// 计算sk并
	int count = 1;
	for(int i = 0; i < 256; i++) if(skb[i] != skb[i+1])count++;
	float s[256] = {0};
	int ns[256] = {0};
	float ps[256] = {0};
	int i = 0;
	for(int j =0; j<count; j++)
	{
		while(skb[i] == skb[i+1]){i++; ns[j] += nr[i];}// 计算sk,nsk
		s[j] = skb[i];
		ps[j] = ns[j]/65536;// 计算psk
	}
	FILE* fp2 = fopen("junhenghua.txt","rt" );
	for(int i =0; i<count; i++)fprintf(fp2,"%d, %d, %f\n", i, ns[i], ps[i]);
	
	fclose(fp1);
	fclose(fp2);
}