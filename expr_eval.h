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
int char_validation(char c);
void cleanup(struct Part *p);
float eval(char *input);
#endif