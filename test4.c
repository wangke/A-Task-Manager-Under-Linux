#include <stdio.h>

int main()
{
	char *s[2];
	s[0] = "haha";
	char *l = s[0];
	printf("%c\n", l[0]);
	printf("%c\n", s[0][0]);
}
