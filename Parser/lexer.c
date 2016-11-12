#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

#define MAX_LINE 50

int lex(FILE *file);
void lex_line(char *line);
int getToken(char *s, Token *token);
char *read_bytes(char *s, char *t, int num);
int is_word(char *s);

char *keywords[] = 
{
	"for",
	"if",
	"while"
};

int main(int argc, char *argv[]){
	lex(fopen(argv[1], "r"));
}

int lex(FILE *file){
	char line[MAX_LINE];

	while(fgets(line, MAX_LINE - 1, file) != NULL){
		//printf("The word is of length %d\n", is_word(line));
		lex_line(line);
	}
}

void lex_line(char *line){
	static int count = 0;

	while(*line){
		Token token;
		++count;
		int length = getToken(line, &token);
		line += length;
		printf("%d\n", length);
	}
}

int getToken(char *s, Token *token){
	int length = 0;

	if(length = is_word(s)){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		printf("The word is of length %d\n", length);
		read_bytes(s, token->value, length);
	}

	return length;
}

char *read_bytes(char *s, char *t, int num){
	while((*s++ = *t++) && --num)
		;
	*t = '\0';
	return t;
}

int is_word(char *s){ //CHECK IF THIS IS SLOW
	int count = 1;
	while(IS_SPACE(*s))
		++s;
	if(!isalpha(*s++))
		return 0;
	while(isalnum(*s++))
		++count;

	return count;
}

int is_number(char *s){

}