#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int is_space(char c){
	return (c==' '?1:0);
}
int main(int argc, char * argv[])
{
	printf("Enter a string\n");
	char buf[32];
//	scanf("%[^\n]", userInput);
	fgets(buf, sizeof(buf), stdin);
	printf("you entered:\n%s\n", buf);
	char copy[32];
	char *ptr = copy;
	for(int i=0; i<strlen(buf); i++){
		if (!is_space(buf[i])){
			*ptr = buf[i];
			++ptr;
		}
	}
	printf("Stripped string %s\nhas length %u\n", copy, strlen(copy));
	for(int i=0; i<strlen(copy); i++){
		printf("%u %u\n", i, copy[i]);
	}
	return 0;
}