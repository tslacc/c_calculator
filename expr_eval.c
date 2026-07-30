#include "expr_eval.h"

struct Part{
	char operator;
	float value;
	struct Part *next;	
};

static int operator_precedence(char c){
	if(c=='^') return 4;
	if(c=='x'||c=='/') return 3;
	if(c=='+'||c=='-') return 2;
	return 0;
}
static int char_validation(char c){
	if('0'<c && c<'9') return 1;
	if(c == '.') return 1;
	if(c == '+' || c == '-' || c == '*' || c == '/' || c=='^') return 2;
	if(c == '(' || c == ')') return 2;
	return 0;
}

static void cleanup(struct Part *p){
	if(p->next!=NULL){
		cleanup(p->next);
	}
	free(p);
	return;
}

//Strip the buffer of spaces and non valid chars
static void strip_spaces(char *input){
	char *ptr = input;
	for(int i=0; i<strlen(input); i++){
		if (char_validation(input[i])){
			*ptr = input[i];
			++ptr;
		}
	}
	*ptr = '\0';
	return;
}

static struct Part *buffer_to_parts(char *input){
	struct Part *unread_head = malloc(sizeof(struct Part));
	struct Part *work = unread_head;
	while(*input!='\0'){
		switch (char_validation(*input)){
			case 1:
				//make a substring that contains the entire character and close the program if the number is invalid.
				char *begin_int = input;
				int periodSeen = 0;
				int length = 0;
				while(char_validation(*input)==1){
					length++;
					if(*input=='.') {
						periodSeen++;
						if(periodSeen>1) {
							printf("Syntax error");
							return 0;
						}
					}
					input++;
				}
				char *int_convert = malloc(length+1);
				memcpy(int_convert, begin_int, length);
				int_convert[length] = '\0';
				work->next = malloc(sizeof(struct Part));
				//printf("work next is new ptr %u\n",work->next);
				work->next->operator = 0;
				//printf("Source string %s\n",int_convert);
				work->next->value = atof(int_convert);
				work->next->next = NULL;
				//printf("Assigning value address is %u, assigning value %f with result %f\n", &(work->next->value), atof(int_convert), work->next->value);
				work = work->next;
				free(int_convert);
				break;
			case 2:
				work->next = malloc(sizeof(struct Part));
				//printf("work next is new ptr %u\n",work->next);
				work->next->operator = *input;
				work->next->value = 0;
				work->next->next = NULL;
				//printf("op in work next set to %c\n", work->next->operator);
				work = work->next;
				input++;
				break;
			default: //error
				cleanup(unread_head);
				return NULL;
		}
	}
	return unread_head->next;
}

//Shunting yard algorithm
//https://en.wikipedia.org/wiki/Shunting_yard_algorithm#The_algorithm_in_detail
float eval_expr(char *input){
	strip_spaces(input);
	struct Part *base = buffer_to_parts(input);
	struct Part *work = base;
	int max_sz = 0;
	while(work!=NULL){
		max_sz++;
		work = work->next;
	}
	work = base;
	
	struct Part *op_stack = NULL;
	struct Part *out_queue_front = NULL;
	struct Part *out_queue_back = NULL;
	
	while(work!=NULL){
		if(work->operator==0){
			printf("Number %f to output queue\n", work->value);
			if(out_queue_front==NULL){
				out_queue_front = work;
				out_queue_back = work;
			} else {
				out_queue_back->next = work;
			}
			work = work->next;
		} else if(work->operator=='('){ //push onto operator stack
			struct Part *tmp = work;
			work = work->next;
			tmp->next = op_stack;
			op_stack = tmp;
		} else if(work->operator==')'){ //Move everything from operator stack to output queue until you find a left paren
			while(op_stack != NULL && op_stack->operator!='('){
				if(out_queue_front==NULL){
					out_queue_front = op_stack;
					out_queue_back = op_stack;
				} else {
					out_queue_back->next = op_stack;
				}
				op_stack = op_stack->next;
			}
			if(op_stack!=NULL && op_stack->operator == '('){
				op_stack = op_stack->next;
			} else {
				//TODO Handle critical input error mismatched parens
				break;
			}
		} else {//This is some kind of operator called o1
			printf("Operator %c, perform operation\n", work->operator);
			//while (
				//there is an operator o2 at the top of the operator stack which is not a left parenthesis
				//and (o2 has greater precedence than o1 
				//or (o1 and o2 have the same precedence and o1 is left-associative))
			while(op_stack!=NULL && op_stack->operator != '(' 
					&& operator_precedence(op_stack->operator) > operator_precedence(work->operator)) {
					// pop o2 from the operator stack into the output queue
					if(out_queue_front==NULL){
						out_queue_front = op_stack;
						out_queue_back = op_stack;
					} else {
						out_queue_back->next = op_stack;
					}
					op_stack = op_stack->next;
					//push o1 onto the operator stack
					struct Part *tmp = work;
					work = work->next;
					tmp->next = op_stack;
					op_stack = tmp;
			}
			
		}
	}
	printf("\n");
	//Cleanup all tokens in the final list
	cleanup(out_queue_front);
	return 0;
}