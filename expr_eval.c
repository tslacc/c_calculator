#include "expr_eval.h"

struct Part{
	char operator;
	float value;
	struct Part *next;	
};
static int left_associative_operator(char c){
	if(c=='^') return 0;
	if(c=='*'||c=='/') return 1;
	if(c=='+'||c=='-') return 1;
	return 0;
}
static int operator_precedence(char c){
	if(c=='^') return 4;
	if(c=='*'||c=='/') return 3;
	if(c=='+'||c=='-') return 2;
	return 0;
}
static int char_validation(char c){
	if('0'<=c && c<='9') return 1;
	if(c == '.') return 1;
	if(c == '+' || c == '-' || c == '*' || c == '/' || c=='^') return 2;
	if(c == '(' || c == ')') return 2;
	return 0;
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
	int negative_ok = 1;
	printf("Rcvd input %s\n", input);
	while(*input!='\0'){
		int test = char_validation(*input);
		printf("begin loop reading char %c\n", *input);
		if(test == 1 || (negative_ok && *input == '-')){
			//make a substring that contains the entire character and close the program if the number is invalid.
			char *begin_int = input;
			int periodSeen = 0;
			int length = 0;
			if(*input == '-') {
				length++;
				input++;
			}
			while(char_validation(*input)==1){
				length++;
				if(*input=='.') {
					periodSeen++;
					if(periodSeen>1) {
						//printf("Syntax error");
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
			printf("attempt print\n");
			printf("Alloc part %p, (%u,%c) %f\n", work, work->operator, work->operator, work->value);
			free(int_convert);
			negative_ok == 0;
		}
		else if(test == 2) {
			if(*input == '(') negative_ok = 1;
			work->next = malloc(sizeof(struct Part));
			//printf("work next is new ptr %u\n",work->next);
			work->next->operator = *input;
			work->next->value = 0;
			work->next->next = NULL;
			//printf("op in work next set to %c\n", work->next->operator);
			work = work->next;
			printf("Alloc part %p, (%u,%c) %f\n", work, work->operator, work->operator, work->value);
			input++;
		}
		else { //Input error crash program
			;
		}
		printf("About to resume loop with input at %c\n", *input);
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
	printf("\nBegin process\n");
	while(work!=NULL){
		printf("Process unit %p, (%u, %c), %f\n", work, work->operator, work->operator, work->value);
		if(work->operator==0){
			printf("\t# branch\n");
			printf("\t%p, (%u,%c), %f to output_queue\n", work, work->operator, work->operator, work->value);
			if(out_queue_front == NULL){
				out_queue_front = out_queue_back = work;
			} else {
				out_queue_back->next = work;
				out_queue_back = work;
			}
			work=work->next;
			//Unlink new addition to the queue to avoid loops.
			out_queue_back->next=NULL;
		} else if(work->operator=='('){ //push onto operator stack
			printf("\t( branch\n");
			printf("\t%p, (%u,%c), %f to op_stack\n", work, work->operator, work->operator, work->value);
			if(op_stack==NULL){
				op_stack = work;
				work=work->next;
				op_stack->next = NULL;
			} else {
				struct Part *tmp = work;
				work = work->next;
				tmp->next = op_stack;
				op_stack = tmp;

			}
			
		} else if(work->operator==')'){ //Move everything from operator stack to output queue until you find a left paren
			printf("\t) branch, move all from op stack to out queue until left paren found\n");
			while(op_stack != NULL && op_stack->operator!='('){
				printf("\t%p, (%u,%c), %f from op_stack to op_queue\n", op_stack, op_stack->operator, op_stack->operator, op_stack->value);
				if(out_queue_front==NULL){
					out_queue_front = op_stack;
					out_queue_back = op_stack;
				} else {
					out_queue_back->next = op_stack;
					out_queue_back = op_stack;
				}
				op_stack = op_stack->next;
				out_queue_back->next = NULL;
			}
			if(op_stack!=NULL && op_stack->operator == '('){
				printf("\tDiscard ( %p\n", op_stack);
				op_stack = op_stack->next;
			} else {
				printf("\nParse error: Critical mismatched parens\n");
				struct Part *tmp;
				while(op_stack!=NULL){
					tmp = op_stack;
					if(op_stack->next == NULL){
						free(tmp);
						break;						
					}
					op_stack = op_stack->next;
					free(tmp);
				}
				while(out_queue_front!=NULL){
					tmp = out_queue_front;
					if(out_queue_front->next == NULL){
						free(tmp);
						break;						
					}
					out_queue_front = out_queue_front->next;
					free(tmp);
				}
				return 0;
				break;
			}
			printf("\tAdvance work\n");
			work = work->next;
		} else {//This is some kind of operator called o1
			printf("\tOperator branch\n", work->operator);
			//while (
				//there is an operator o2 at the top of the operator stack which is not a left parenthesis
				//and (o2 has greater precedence than o1 
				//or (o1 and o2 have the same precedence and o1 is left-associative))
			while(
				op_stack!=NULL && op_stack->operator != '(' 
					&& (
						operator_precedence(op_stack->operator) > operator_precedence(work->operator)
						|| (
							operator_precedence(op_stack->operator) == operator_precedence(work->operator)
							&& left_associative_operator(work->operator)
						)
					) 
				){
				printf("\tPrecedence of o1 o2, %c %c\n", work->operator, op_stack->operator);

				// pop o2 from the operator stack into the output queue
				if(out_queue_front==NULL){
					out_queue_front = op_stack;
					out_queue_back = op_stack;
				} else {
					out_queue_back->next = op_stack;
					out_queue_back = op_stack;
				}
				op_stack = op_stack->next;		
			}
			//push o1 onto the operator stack
			printf("\t%p, (%u,%c), %f to op_stack\n", work, work->operator, work->operator, work->value);
			struct Part *tmp = work;
			work = work->next;
			tmp->next = op_stack;
			op_stack = tmp;
		}
	}
	while(op_stack!=NULL){
		printf("Op stack contains %u (%u, %c), %f\n", op_stack, op_stack->operator, op_stack->operator, op_stack->value);
		if(op_stack->operator=='(') {//CRITICAL ERROR
			break;
		}
		if(out_queue_front==NULL){
			out_queue_front = op_stack;
			out_queue_back = op_stack;
		} else {
			out_queue_back->next = op_stack;
			out_queue_back = op_stack;
		}
		if(op_stack->next == NULL){
			break;
		}
		op_stack = op_stack->next;

	}
	printf("Done making rpn\n");
	printf("%u, %u\n", out_queue_front, out_queue_back);
	struct Part *placeholder = out_queue_front;
	while(out_queue_front!=NULL){
		printf("Object at %p contains (%u,%c) %f \n", out_queue_front, out_queue_front->operator, out_queue_front->operator, out_queue_front->value);
		if(out_queue_front->next == NULL) break;
		out_queue_front = out_queue_front->next;
	}
	out_queue_front = placeholder;
	//printf("build eval stack\n");
	struct Part *eval_stack = out_queue_front;
	//printf("Add %f to eval stack\n", out_queue_front->value);
	out_queue_front = out_queue_front->next;
	while(out_queue_front!=NULL){
		//printf("Checking unit %u (%u,%c) %f\n", out_queue_front, out_queue_front->operator, out_queue_front->operator, out_queue_front->value);
		if(out_queue_front->operator == 0) {
			//printf("Add %f to eval stack\n", out_queue_front->value);
			struct Part *tmp = out_queue_front;
			out_queue_front = out_queue_front->next;
			tmp->next = eval_stack;
			eval_stack = tmp;
		} else {
			struct Part *op1 = eval_stack;
			eval_stack = eval_stack->next;
			//printf("Evaluating operator %c in %u %u, v %f %f\n", out_queue_front->operator, op1, eval_stack, op1->value, eval_stack->value);
			switch (out_queue_front->operator) {
				case '+':
					eval_stack->value = eval_stack->value + op1->value;
					break;
				case '-':
					eval_stack->value = eval_stack->value - op1->value;
					break;
				case '*':
					eval_stack->value = eval_stack->value * op1->value;
					break;
				case '/':
					eval_stack->value = eval_stack->value / op1->value;
					break;
			}
			free(op1);
			struct Part *tmp = out_queue_front;
			out_queue_front = out_queue_front->next;
			free(tmp);
		}
		
	}

	float result = eval_stack->value;
	//printf("r = %f, free eval stack %u\n", result, eval_stack);
	free(eval_stack);
	return result;
}