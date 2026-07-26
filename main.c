#include <stdlib.h>
#include <stdio.h>

#include "expr_eval.h"

#define BUFFER_SZ 64
//Shunting yard algorithm
//https://en.wikipedia.org/wiki/Shunting_yard_algorithm#The_algorithm_in_detail
int main(int argc, char * argv[])
{
	printf("Enter a string\n");
	char buf[BUFFER_SZ];
//	scanf("%[^\n]", userInput);
	fgets(buf, sizeof(buf), stdin);
	printf("you entered:\n%s\n", buf);
	printf("Eval returns %f\n", eval(buf));
	return 0;
}
