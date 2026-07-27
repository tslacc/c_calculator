#ifndef EXPR_PART_
#define EXPR_PART_
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct Part{
	char operator;
	float value;
	struct Part *next;	
};
void cleanup(struct Part *p);
float eval_expr(char *input);
#endif