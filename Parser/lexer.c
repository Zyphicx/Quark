#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#define MAX_LINE 512

void lex(FILE *file, Token_List *list);
char *get_next_token(char *line, Token *token);
int read_token(char *s, char *t, int len);
int is_word(char *s);

char *keywords[] = {"for",
			"if",
			"while"};

int main(int argc, char *argv[]){
	Token_List list;
	new_list(&list, 5);
	Token token;
	token.value = "Lol\n";
	add_entry(&list, token);

	int i;
	for(i = 0; i < list.next; i++){
		printf("%s", list.tokens[i].value);
	}
	if(argc == 2)
		lex(fopen(argv[1], "r"), &list);
}

void lex(FILE *file, Token_List *list){
	int linecount = 0;
	char line[MAX_LINE]; //It will end with a \0, so it won't read from old lines
	char *line_ptr;
	while(fgets(line, MAX_LINE - 1, file) != NULL){
		line_ptr = line;
		Token *token;

		while(*line_ptr)
			line_ptr = get_next_token(line_ptr, token);
	}
}

char *get_next_token(char *line, Token *token){
	token = (Token *)malloc(sizeof(Token));

	//Check for the following and set token type

	//Check for words
	int length;

	if(length = is_word(line)){
		token->value = (char *)malloc(length * sizeof(char) + 1); //Add 1 for null character
		read_token(line, token->value, length);
		line += length + 1;
		printf("The word is \"%s\", and it is of length: %d\n", token->value, length);
	}

	//Check for numbers

	//Check for specific characters

	//Malloc token char and set value
	return line;
}

int read_token(char *s, char *t, int len){
	while(isspace(*s))
		++s;
	while(len-- > 0){
		*t++ = *s++;
	}
	*++t = '\0';
}

int is_word(char *s){
	while(isspace(*s))
		++s;
	int count = 0;
	if(!isalpha(*s++))
		return 0;
	++count;
	while(isalnum(*s++))
		++count;
	return count;
}