#ifndef EXPR_PART_
#define EXPR_PART_
#include <stddef.h>
#include <stdlib.h>
struct Part{
	char operator;
	float value;
	struct Part *next;	
};
int char_validation(char c);
void cleanup(struct Part *p);
#endif