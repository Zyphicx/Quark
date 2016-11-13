#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

#define MAX_LINE 150

int lex(FILE *file);
int getToken(char *s, Token *token);
char *read_bytes(char *s, char *t, int num);
int is_word(char *s);
int is_number(char *s);
int is_preprocessor_instruction(char *s);
int is_symbols(char *s, enum token_type *type);
enum token_type get_keyword_type(char *s);

struct key_type_pair keywords[] = {
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
			if(!(*line_ptr))
				break;
			Token *token = malloc(sizeof(Token));
			length = getToken(line_ptr, token);
			line_ptr += length;
			//printf("%s\n", line_ptr);
			//printf("%d\n", length);
			if(token->type != UNDEFINED)
				free(token->value);
			free(token);
		}
	}
	printf("Done!\n");
}

int getToken(char *s, Token *token){
	int length = 1;
	int space = 0;
	while(isspace(*s)){
		++s;
		++space;
	}

	if(length = is_word(s)){
		printf("I am a word\n");
		token->value = (char *)malloc(length + 1); //Add one for the null character
		read_bytes(s, token->value, length);
		token->type = get_keyword_type(token->value);
	}
	else if(length = is_number(s)){
		printf("I am a number\n");
		token->value = (char *)malloc(length + 1); //Add one for the null character
		read_bytes(s, token->value, length);
		token->type = NUMBER;
	}
	else if(*s == '#'){
		printf("I am preprocessor\n");
		token->value = (char *)malloc(length + 1); //Add one for the null character
		read_bytes(s, token->value, length);
		token->type = PREPROCESSOR;
	}
	else if(length = is_symbols(s, &(token->type))){
		printf("I am a symbol\n");
		//printf("Symbol length: %d\n", length);
		token->value = (char *)malloc(length + 1); //Add one for the null character
		read_bytes(s, token->value, length);
	}else{
		token->type = UNDEFINED;
		printf("I am nothing :(\n");
	}

	printf("%s\t%d\t%ld\t%d\n", token->value, token->type, strlen(token->value), *token->value);

	return length + space;
}

char *read_bytes(char *s, char *t, int num){
	while(num--)
		(*t++ = *s++);
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
	while(IS_NUM(*s)){
		++count;
		++s;
	}
	if(*s++ != '.')
		return count;
	++count;
	while(IS_NUM(*s)){
		++s;
		++count;
	}

	return count;
}

int is_preprocessor_instruction(char *s){
	if(*s != '#')
		return 0;
	return strlen(s);
}

int is_symbols(char *s, enum token_type *t){
	*t = 0;
	switch(*s){
		case '+': *t = ADD;			break;
		case '-': *t = SUBTRACT;	break;
		case '*': *t = MULTIPLY; 	break;
		case '/': *t = DIVIDE;		break;
		case '%': *t = MODULO;		break;
		case '^': *t = EXPONENT;	break;
		case '>': *t = GREATER;		break;
		case '<': *t = LESS;		break;
		case '!': *t = NOT;			break;
		case '=': *t = ASSIGN;		break;
		case '&': *t = AMPER;		break;
		case '|': *t = VBAR;		break;
		case '(': *t = L_PAREN;		break;
		case ')': *t = R_PAREN;		break;
		case '{': *t = L_CURLY;		break;
		case '}': *t = R_CURLY;		break;
		case ',': *t = COMMA;		break;
		case ':': *t = COLON;		break;
		case ';': *t = SEMICOLON;	break;
	}

	switch(*(s+1)){
		case '=':
			switch(*t){
				case ADD: *t = ADD_ASSIGN;				return 2;
				case SUBTRACT: *t = SUBTRACT_ASSIGN;	return 2;
				case MULTIPLY: *t = MULTIPLY_ASSIGN;	return 2;
				case DIVIDE: *t = DIVIDE_ASSIGN;		return 2;
				case MODULO: *t = MODULO_ASSIGN;		return 2;
				case EXPONENT: *t = EXPONENT_ASSIGN;	return 2;
				case GREATER: *t = GREATER_EQUALS;		return 2;
				case LESS: *t = LESS_EQUALS;			return 2;
				case NOT: *t = NOT_EQUALS;				return 2;
				default: return 1;
		}
		case '&':
			switch(*t){
				case AMPER: *t = AND;					return 2;
				default: return 1;						break;
			}
		return 2;
	}
	return *t ? 1 : 0;
}

enum token_type get_keyword_type(char *s){
	struct key_type_pair *kp;
	for(kp = keywords; kp < (keywords + keywords_amount); kp++){
		if(!strcmp(s, kp->key)){
			return kp->type;
		}
	}
	return IDENTIFIER;
}