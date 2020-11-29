#include <stdio.h>

int main()
{
	char *str = "hello";
	int num = 999;
	int count = 0;

	printf("---------------normal ret----------------\n");
	printf("123456789012345678901234567890\n");
	count = printf("%s", str);
	printf("\nret:%d\n\n", count);
	printf("123456789012345678901234567890\n");
	count = printf("%d", num);
	printf("\nret:%d\n", count);

	printf("\n---------------false ret---------------\n");
	printf("123456789012345678901234567890\n");
	count = printf("%s", str);
	printf("\nret:%d\n\n", count);
}
