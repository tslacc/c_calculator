#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
	//Parse all and add to read.
	ptr = copy;
	struct Part *unread_head = malloc(sizeof(struct Part));
	struct Part *work = unread_head;
	while(*ptr!='\0'){
		switch (char_validation(*ptr)){
			case 1:
				//make a substring that contains the entire character and close the program if the number is invalid.
				char *begin_int = ptr;
				int periodSeen = 0;
				int length = 0;
				while(char_validation(*ptr)==1){
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
				work->next->operator = *ptr;
				work->next->value = 0;
				work->next->next = NULL;
				//printf("op in work next set to %c\n", work->next->operator);
				work = work->next;
				ptr++;
				break;
			default: //error
				return 0;
		}
	}
	work = unread_head->next;

	while(work!=NULL){
		//printf("\nwork node ptr is %u\n", work);
		//printf("op %c, val %f\n", work->operator, work->value);
		//printf("Address reading from is %u, %u\n", &(work->operator), &(work->value));
		if(work->operator==0) printf("%f", work->value);
		else printf("%c", work->operator);
		if(work->next == NULL) break;
		work=work->next;
	}
	printf("\n");
	cleanup(unread_head);
	return 0;
}
