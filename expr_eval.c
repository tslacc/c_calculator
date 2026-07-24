#include "expr_eval.h"

int char_validation(char c){

	if('0'<c && c<'9') return 1;
	if(c == '.') return 1;
	if(c == '+' || c == '-' || c == '*' || c == '/') return 2;
	if(c == '(' || c == ')') return 2;
	return 0;
}

void cleanup(struct Part *p){
	if(p->next!=NULL){
		cleanup(p->next);
	}
	free(p);
	return;
}
