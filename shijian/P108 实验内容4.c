#include<stdio.h>
#include<math.h>
double x,sum=0,difference=1;
int t=1,i,result;
int main()
{
	printf("������x��ֵ��\n");
	scanf("%lf",&x);
	while(difference>1e-6)
	{
		result=1;
		for(i=1;i<=(2*t-1);i++)
			result=result*i;
		sum=sum+pow(-1,t+1)*pow(x,2*t-1)/result;
		difference=fabs(sum-sin(x));
		t++;
	}
	printf("sinx�Ľ���ֵ��%lf",sum);
	return 0;
} 
