#include<stdio.h>
#include<math.h>
int main()
{
	float a,b,c,p,S;
	printf("�����������ε����߳���\n");
	scanf("%f%f%f",&a,&b,&c);
	if(a>0&&b>0&&c>0)
	{
		if(a+b>c&&a+c>b&&b+c>a)
		{ 
			if(a==b&&b==c)
				printf("�ȱ�������\n");
			if((a==b&&a!=c)||(a==c&&a!=b)||(b==c&&b!=a))
				printf("����������\n");
			if(a*a+b*b==c*c||a*a+c*c==b*b||b*b+c*c==a*a)
				printf("ֱ��������\n");
			p=(a+b+c)/2;
			S=sqrt(p*(p-a)*(p-b)*(p-c));
			printf("�����ε������%f\n",S);
		}
		else
			printf("�޷�����������\n");
	}
	else
		printf("DATA ERROR!\n");
	return 0;
}
