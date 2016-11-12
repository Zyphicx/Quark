#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

#define MAX_LINE 53

int lex(FILE *file);
int getToken(char *s, Token *token);
char *read_bytes(char *s, char *t, int num);
int is_word(char *s);

const struct keyword keywords[] = {
	"del", DELETE,
	"for", FOR,
	"if", IF,
	"include", INCLUDE,
	"while", WHILE
};

int main(int argc, char *argv[]){
	lex(fopen(argv[1], "r"));
}

int lex(FILE *file){
	int length;
	char line[MAX_LINE];
	char *line_ptr;

	while(fgets(line, MAX_LINE, file) != NULL){
		line_ptr = line;

		while(*line_ptr){
			Token token;
			length = getToken(line_ptr, &token);
			line_ptr += length;
			//printf("%d\n", length);
		}
	}
	printf("Done!\n");
}

int getToken(char *s, Token *token){
	int length = 0;
	while(isspace(*s))
		++s;

	if(length = is_word(s)){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		//printf("The word is of length %d\n", length);
		read_bytes(s, token->value, length);
	}
	else if(length = is_number(%s)){
		token->type = NUMBER;
		token->value = (char *)malloc(length + 1);
		read_bytes(s, token->value, length);
	}

	return length + 1;
}

char *read_bytes(char *s, char *t, int num){
	while((*s++ = *t++) && --num)
		;
	*t = '\0';
	return t;
}

int is_word(char *s){ //CHECK IF THIS IS SLOW
	int count = 1;
	if(!isalpha(*s++))
		return 0;
	while(isalnum(*s++))
		++count;

	return count;
}

int is_number(char *s){
	int count = (*s == '-' ? 1 : 0);
	s += count;
	while(isdigit(*s++))
		++count;
	if(*s++ != '.')
		return count;
	++count;
	while(isdigit(*s++))
		++count;

	return count;
}