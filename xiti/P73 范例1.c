#include<stdio.h>
int main()
{
	long i,n;
	double pi,di;
	printf("������ pi ����ֵ�ĵ���������\n");
	scanf("%ld",&n);
	pi=2.0;
	di=1.0;
	for(i=1;i<=n;i++)
	{
		pi*=4*di*di/(2*di-1)/(2*di+1);
		di+=1;
	}
	printf("����%ld �εĵ�����pi �Ľ���ֵΪ:%.15f\n",n,pi);
	return 0;
}
