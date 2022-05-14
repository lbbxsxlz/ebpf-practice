#include <stdio.h>
#include <stdlib.h>

int myadd(int a, int b)
{
	return a + b;
}

int main(int argc, char** argv)
{
	int x = 2, y= 3;
	int sum = myadd(x, y);
	printf("sum = %d \n", sum);
	return 0;
}
