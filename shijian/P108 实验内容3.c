#include<stdio.h>
#include<stdlib.h>
int main()
{
	char text; 
	int num_letter=0,num_space=0,num_table=0,num_enter=0,num_else=0;
	printf("���������ģ�\n");
	while((text=getchar())!=EOF)
	{
		if((text>='a'&&text<='z')||(text>='A'&&text<='Z'))
			num_letter++;
		else if(text==' ')
			num_space++;
		else if(text=='	')
			num_table++;
		else if(text=='\n')
			num_enter++;
		else
			num_else++;
	}
	printf("Ӣ����ĸ:%d\n�ո�:%d\n�Ʊ��:%d\n���з�:%d\n�����ַ�:%d\n",num_letter,num_space,num_table,num_enter,num_else);
	return 0;
}
