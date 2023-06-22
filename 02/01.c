#include <stdio.h>

int main()
{
	int i,j,s,num;
	printf("Please input raw: ");
	scanf("%d",&num);
	for(i = 0;i < num;i ++)
	{
		for(j = 1;j < num - i;j ++)
		{
			printf(" ");
		}
		for(s = 0;s < i * 2 + 1;s ++)
		{
			printf("*");
		}
		printf("\n");
	}
}
