#include<stdio.h>
int main()
{
	int x[4][4],i,j;
	long djh=0,djj=1,kbh=0,bkbh=0,kbj=1,bkbj=1;
	for(i=0;i<4;i++)
	{	printf("�������%d��:",i+1);
		for(j=0;j<4;j++)
			scanf("%d",&x[i][j]);
	}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			if(i==j||i+j==3)
			{
				djh+=x[i][j];
				djj+=x[i][j];
			}
			if(i==0||i==3||j==0||j==3)
			{
				kbh+=x[i][j];
				kbj*=x[i][j];
			} 
			if(i!=0&&i!=3&&j!=0&&j!=3)
			{
				bkbh+=x[i][j];
				bkbj*=x[i][j]; 
			}
		}
	printf("�Խ���Ԫ�غ�=%ld,�Խ���Ԫ�ػ�=%ld\n",djh,djj);
	printf("����Ԫ�صĺ�=%ld,����Ԫ�صĻ�=%ld\n",kbh,kbj);
	printf("������Ԫ�صĺ�=%ld,������Ԫ�صĻ�=%ld\n",bkbh,bkbj);
}
