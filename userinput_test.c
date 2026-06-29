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
	//Strip the ints of spaces and non valid chars
	for(int i=0; i<strlen(buf); i++){
		if (char_validation(buf[i])){
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
				//make a substring that contains the entire character and close the program if the number is invalid.
				char *begin_int = ptr;
				int periodSeen = 0;
				int length = 0;
				while(char_validation(*ptr)==1){
					printf("char of *ptr = %c\n", *ptr);
					length++;
					if(*ptr=='.') {
						periodSeen++;
						if(periodSeen>1) {
							printf("Syntax error");
							return 0;
						}
					}
					ptr++;
				}
				char *int_convert = malloc(length+1);
				memcpy(int_convert, begin_int, length);
				int_convert[length] = '\0';
				printf("Parsed ascii num = %d\n", atol(int_convert));
				free(int_convert);
				break;
			case 2:
				struct Part *next = malloc(sizeof(struct Part));
				next->value = 0;
				next->next = NULL;
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
	work = unread_head->next;
	printf("work node ptr is %u\n", work);
	while(work!=NULL){
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