#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SZ 64
//A stack component. Either operator is 0 and this is a number, or operator is nonzero and is an operator (+-*/^)
struct Part{
	char operator;
	long value;
	struct Part *next;	
};
int char_validation(char c){

	if('0'<c && c<'9') return 1;
	if(c == '.') return 1;
	if(c == '+' || c == '-' || c == '*' || c == '/') return 2;
	if(c == '(' || c == ')') return 2;
	return 0;
}
int main(int argc, char * argv[])
{
	printf("Enter a string\n");
	char buf[BUFFER_SZ];
//	scanf("%[^\n]", userInput);
	fgets(buf, sizeof(buf), stdin);
	printf("you entered:\n%s\n", buf);
	char copy[32];
	char *ptr = copy;
	//Strip the ints
	for(int i=0; i<strlen(buf); i++){
		printf("validation is %u\n", char_validation(buf[i]));
		if (char_validation(buf[i])){
			printf("adding char %c\n", buf[i]);
			*ptr = buf[i];
			++ptr;
		}
	}
	*ptr = '\0';
	printf("Stripped string %s\nhas length %u\n", copy, strlen(copy));
	//Parse all and add to read.
	ptr = copy;
	struct Part *unread_head = malloc(sizeof(struct Part));
	printf("unread head ptr %u\n", unread_head);
	struct Part *work = unread_head;
	while(*ptr!='\0'){
		printf("Scanning ptr component %c\n", *ptr);
		
		switch (char_validation(*ptr)){
			case 1:
				ptr++;
				break;
			case 2:
				struct Part *next = malloc(sizeof(struct Part));
				next->value = 0;
				next->next = NULL;
				printf("Case 2 operating on object %u\n", next);
				printf("work pointer %u\n", work);				
				printf("next pointer %u\n", next);
				work->next = next;
				next->operator = *ptr;
				
				ptr++;
				break;
			default: //error
				return 0;
		}
	}
	printf("unread head ptr %u\n", unread_head);
	printf("unread head next ptr %u\n", unread_head->next);
	printf("unread head next ptr deref %u\n", *unread_head->next);
	work = unread_head->next;
	printf("work node ptr is %u\n", work);
	while(&work!=NULL){
		printf("work contains op %u\n", work->operator);
		if(work->next == NULL) break;
		work=work->next;
	}
	printf("End program - has not yet cleared malloced Parts\n");
	return 0;
}
//TODO Destroy all spaces AND non-numeric, non-operand keys.
//Shunting yard algorithm
//https://en.wikipedia.org/wiki/Shunting_yard_algorithm#The_algorithm_in_detail