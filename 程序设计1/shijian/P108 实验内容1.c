#include<stdio.h>
int main()
{
	int m,n,r,max,min,mm,nn,temp,pemt;
	r=1;
	printf("�����Լ������С������\n������������������\n");
	scanf("%d%d",&m,&n);
	if(m<n) 
	{
		temp=m;
		m=n;
		n=temp;
	}
	mm=m;
	nn=n;
	while(r!=0)
	{
		r=m%n;
		m=n;
		pemt=n;
		n=r;
	}
	n=pemt;
	max=n;
	min=mm*nn/max;
	printf("���Լ��Ϊ%d  ��С������Ϊ%d\n",max,min);
	return 0;
}
