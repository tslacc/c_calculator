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
	struct Part **output_queue = malloc(sizeof(struct Part *)*max_sz);
	int output_queue_position = 0;
	int output_queue_size = 0;
	struct Part **operator_stack = malloc(sizeof(struct Part *)*max_sz);
	int operator_stack_size = 0;
	
	while(work!=NULL){
		if(work->operator==0){
			printf("Number %f to output queue\n", work->value);
			output_queue[output_queue_size] = work;
			output_queue_size++;
		} else if(work->operator=='('){ //push onto operator stack
			operator_stack[operator_stack_size]=work;
			operator_stack_size++;
		} else if(work->operator==')'){ //Move everything from operator stack to output queue until you find a left paren
			while(operator_stack_size != 0 && operator_stack[operator_stack_size]->operator != '('){
				output_queue[output_queue_size] = operator_stack[operator_stack_size];
				output_queue_size++;
				operator_stack_size--;
			}
			if(operator_stack_size != 0 && operator_stack[operator_stack_size]->operator == '('){
				operator_stack_size--;
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
			while(operator_stack_size>0 && operator_stack[operator_stack_size-1]->operator != '(' 
					&& operator_precedence(operator_stack[operator_stack_size-1]->operator) > operator_precedence(work->operator)) {
					// pop o2 from the operator stack into the output queue
					// push o1 onto the operator stack
					output_queue[output_queue_size] = operator_stack[operator_stack_size];
					output_queue_size++;
					operator_stack_size--;
					//push o1 onto the operator stack
					operator_stack[operator_stack_size] = work;
					operator_stack_size++;
			}
		}
		if(work->next == NULL) break;
		work=work->next;
	}
	printf("\n");
	cleanup(work);
	free(output_queue);
	free(operator_stack);
	return 0;
}