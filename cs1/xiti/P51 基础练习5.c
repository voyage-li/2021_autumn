#include<stdio.h>
#include<math.h>
int main()
{
	float x,y;
	printf("����������������������\n");
	scanf("%f%f",&x,&y);
	if(fabs(x)<=1&&fabs(y)<=1)
		printf("yes\n");
	else
		printf("no\n");
	return 0;
} 
