#include<stdio.h>
int main()
{
	double x,i;
	x=1.5;
	for(i=0;i<500;i++)
		x=(4*x*x*x-4*x*x+6)/(6*x*x-8*x+3); 
	printf("������1.5������ֵΪ%lf",x);
	return 0;
}
