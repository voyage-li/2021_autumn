#include<stdio.h>
int main()
{
	long long int n,i;
	double Pi=2.0; 
	printf("����Pi��ֵ\n");
	printf("�����뾫ȷ�ȣ�\n");
	scanf("%ld",&n);
	for(i=1;i<n;i++)
		Pi=Pi*(2*i)*(2*i)/((2*i-1)*(2*i+1));
	printf("Pi�Ľ���ֵ��%lf",Pi);
	return 0;
}
