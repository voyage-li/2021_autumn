#include<stdio.h>
int main()
{
	int n,x,y,i,j;
	printf("������һ�� n ��ƻ�����ܲ��ҵ�������ʱ����������һ�����ӡ�����ÿ x Сʱ�ܳԵ�һ��ƻ������������ڳ���һ��ƻ��֮ǰ�������һ������ô���� y Сʱ�㻹�ж��ٸ�������ƻ����\n");
	scanf("%d%d%d",&n,&x,&y);
	if(y>n*x)
		printf("False!\n");
	else
	{
		if(y%x==0)
		{
			i=y/x;
			j=n-i;
		}
		else
		{
			i=y/x+1;
			j=n-i;
		}
		printf("%d",j);
	}
	return 0;
} 
