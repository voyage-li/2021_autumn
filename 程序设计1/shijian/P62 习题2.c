#include<stdio.h>
#define PI 3.1415926
int main()
{
	float R,H,Volume;
	printf("����������Բ����İ뾶R�͸�H:\n");
	scanf("%f %f",&R,&H);
	if(R>0&&H>0)
	{ 
		Volume=PI*R*R*H;
		printf("Բ��������Volume=%f\n",Volume);
	}
	else
		printf("Erorr!\n"); 
	return 0;
 } 
