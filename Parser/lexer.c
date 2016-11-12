#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

#define MAX_LINE 53

int lex(FILE *file);
int getToken(char *s, Token *token);
char *read_bytes(char *s, char *t, int num);
int is_word(char *s);
int is_number(char *s);

struct keyword keywords[] = {
	"del", DELETE,
	"for", FOR,
	"if", IF,
	"include", INCLUDE,
	"while", WHILE
};

const int keywords_amount = sizeof(keywords) / sizeof(keywords[0]);

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
		struct keyword *kp;
		token->type = IDENTIFIER;
		for(kp = keywords; kp < (keywords + keywords_amount); kp++){
			if(!strcmp(token->value, kp->key)){
				token->type = kp->type;
				break;
			}
		}
	}
	else if(length = is_number(s)){
		token->type = NUMBER;
		token->value = (char *)malloc(length + 1);
		read_bytes(s, token->value, length);
	}

	printf("%s\t%d\t%ld\n", token->value, token->type, strlen(token->value));

	return length + 1;
}

char *read_bytes(char *s, char *t, int num){
	while((*t++ = *s++) && --num)
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
	while(isdigit(*s)){
		++count;
		++s;
	}
	if(*s++ != '.')
		return count;
	++count;
	while(isdigit(*s++))
		++count;

	return count;
}