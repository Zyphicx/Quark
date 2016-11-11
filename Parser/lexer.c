#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"
#define MAX_LINE 50000

void lex(FILE *file, Token_List *list);
int get_next_token(char *line, Token *token);
char *read_line(char *s, int n, FILE *file);
int read_token(char *s, char *t, int len);
int is_word(char *s);

char *keywords[] = 
	{"for",
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
	while(fgets(line, MAX_LINE - 1, file) != NULL){ //Use mmap for reading instead!!!!!!!!!!!!!!!!!!
		line_ptr = line;
		Token *token;

		while(*line_ptr){
			line_ptr += get_next_token(line_ptr, token);
		}
	}
}

int get_next_token(char *s, Token *token){
	token = (Token *)malloc(sizeof(token));

	int length;

	if(length = is_word(s)){
		token->value = malloc(length * sizeof(char) + 1); //Add 1 for null character
		read_token(s, token->value, length);
		printf("The word is \"%s\", and it is of length: %d\n", token->value, length);
	}

	return length + 1;
}

char *read_line(char *s, int n, FILE *file){
	while((*s = getc(file)) != EOF && *s != '\n' && n-- > 0)
		++s;
	*++s = '\0';
	return *--s == EOF ? NULL : s;
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