#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

#include "token.h"
#include "grammar.h"

#define MAX_LINE 4096

Keyword keywords[] = {
	"double", DOUBLE,
	"else", ELSE,
	"for", FOR,
	"if", IF,
	"include", INCLUDE,
	"int", INTEGER,
	"List", LIST,
	"String", STRING,
	"while", WHILE
};

const int keywordAmount = sizeof(keywords) / sizeof(keywords[0]);

int main(int argc, char *argv[]){
	clock_t start = clock();
	setlocale(LC_CTYPE,"it_IT.UTF-8");
	Token *list = lexFile(argv[1]);
	printf("It took %f seconds\n", ((double)(clock() - start) / CLOCKS_PER_SEC));

	Token *curToken;

	while((curToken = list) != NULL){
		list = list->next;
		if(curToken->type != UNDEFINED)
			free(curToken->value);
		free(curToken);
	}

	setupGrammar();

	return 0;
}

Token *lexFile(char *path){
	char line[MAX_LINE];
	char *line_ptr;
	int count = 0;

	Token *head = NULL;
	Token *tail = NULL;

	FILE *file = fopen(path, "r");

	while(fgets(line, MAX_LINE + 1, file) != NULL){
		line_ptr = line;

		while(*line_ptr){
			while(isspace(*line_ptr))
				++line_ptr;

			if(*line_ptr == '\n' || *line_ptr == '\r' || !*line_ptr){
				break;
			}

			Token *token = malloc(sizeof(Token));
			int length = getToken(line_ptr, token);

			line_ptr += length;

			if(token->type != UNDEFINED)
				if(head == NULL){
					head = token;
					tail = token;
				}else{
					tail->next = token;
					tail = token;
				}

			++count;
		}
	}
	printf("Read %d tokens\n", count);

	fclose(file);

	return head;
}

int getToken(char *s, Token *token){
	int length = 1;

	if(length = isWord(s)){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		readBytes(s, token->value, length);
		token->type = getKeywordType(token->value);
	}
	else if(length = isNumber(s)){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		readBytes(s, token->value, length);
		token->type = NUMBER;
	}
	else if(*s == '#'){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		readBytes(s, token->value, length);
		token->type = PREPROCESSOR;
	}
	else if(length = isSymbols(s, &(token->type))){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		readBytes(s, token->value, length);
	}
	else if(length = isString(s)){
		token->value = (char *)malloc(length - 1); //-2 for " and ", then + 1 for the null character
		readBytes(s + 1, token->value, length - 2);
		token->type = STRING_CONST;
	}
	else{
		token->type = UNDEFINED;
		printf("UNDEFINED!\n");
	}

	//printf("%s\t%d\t%ld\t%d\n", token->value, token->type, strlen(token->value), *token->value);

	return length;
}

char *readBytes(char *s, char *t, int num){
	while(num--)
		*t++ = *s++;
	*t = '\0';
	return t;
}

int isWord(char *s){
	int count = 1;
	if(!isalpha(*s++))
		return 0;
	while(isalnum(*s++))
		++count;

	return count;
}

int isNumber(char *s){
	int count = (*s == '-' ? 1 : 0);
	s += count;
	while(isdigit(*s)){
		++count;
		++s;
	}
	if(*s++ != '.')
		return count;
	++count;
	while(isdigit(*s)){
		++s;
		++count;
	}

	return count;
}

int isPreprocessor(char *s){
	if(*s != '#')
		return 0;
	return strlen(s);
}

int isSymbols(char *s, enum TokenType *t){
	*t = 0;
	switch(*s){
		case '+': *t = PLUS;		break;
		case '-': *t = MINUS;		break;
		case '*': *t = STAR; 		break;
		case '/': *t = SLASH;		break;
		case '%': *t = PERCENT;		break;
		case '^': *t = CARET;		break;
		case '>': *t = GREATER;		break;
		case '<': *t = LESS;		break;
		case '!': *t = EXCL;		break;
		case '?': *t = QUESTION;	break;
		case '=': *t = EQUALS;		break;
		case '&': *t = AMPER;		break;
		case '|': *t = VBAR;		break;
		case '(': *t = L_PAREN;		break;
		case ')': *t = R_PAREN;		break;
		case '{': *t = L_CURLY;		break;
		case '}': *t = R_CURLY;		break;
		case '[': *t = L_SQUARE;	break;
		case ']': *t = R_SQUARE;	break;
		case ',': *t = COMMA;		break;
		case ':': *t = COLON;		break;
		case ';': *t = SEMICOLON;	break;
	}

	switch(*(s+1)){
		case '=':
			switch(*t){
				case PLUS: *t = PLUS_EQUALS;		return 2;
				case MINUS: *t = MINUS_EQUALS;		return 2;
				case STAR: *t = STAR_EQUALS;		return 2;
				case SLASH: *t = SLASH_EQUALS;		return 2;
				case PERCENT: *t = PERCENT_EQUALS;	return 2;
				case CARET: *t = CARET_EQUALS;		return 2;
				case GREATER: *t = GREATER_EQUALS;	return 2;
				case LESS: *t = LESS_EQUALS;		return 2;
				case EXCL: *t = EXCL_EQUALS;	return 2;
				case EQUALS: *t = EQUALS_EQUALS; 	return 2;
				default: return 1;
		}
		case '&':
			switch(*t){
				case AMPER: *t = AMPER_AMPER;		return 2;
				default: return 1;	break;
			}
		case '+':
			switch(*t){
				case PLUS: *t = PLUS_PLUS;			return 2;
		}
		case '-':
			switch(*t){
				case MINUS: *t = MINUS_MINUS;		return 2;
			}
		return 2;
	}
	return *t ? 1 : 0;
}

int isString(char *s){
	int count = 2;
	if(*s++ != '"')
		return 0;
	while(*s++ != '"')
		++count;
	return count;
}

enum TokenType getKeywordType(char *s){
	Keyword *kp;
	for(kp = keywords; kp < (keywords + keywordAmount); kp++){
		if(strEquals(s, kp->key)){
			return kp->type;
		}
	}
	return IDENTIFIER;
}

int strEquals(const char *s, const char *t){
	register const char *sp = s;
	register const char *tp = t;

	while(*sp == *tp && *sp){ //If t is \0 but s isn't this would still fail
		++sp;
		++tp;
	}

	return (*sp == *tp ? 1 : 0);
}
