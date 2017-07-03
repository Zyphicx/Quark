#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

#include "token.h"
#include "grammar.h"
#include "lexer.h"

TokenName keywords[] = {
	"List", LIST,
	"String", STRING,
	"Tuple", TUPLE,
	"attribute", ATTRIBUTE,
	"break", BREAK,
	"case", CASE,
	"class", CLASS,
	"const", CONST,
	"continue", CONTINUE,
	"double", DOUBLE,
	"else", ELSE,
	"enum", ENUM,
	"false", FALS,
	"for", FOR,
	"from", FROM,
	"if", IF,
	"include", INCLUDE,
	"int", INTEGER,
	"is", IS,
	"match", MATCH,
	"return", RETURN,
	"self", SELF,
	"super", SUPER,
	"switch", SWITCH,
	"true", TRU,
	"type", TYPE,
	"void", VOID,
	"while", WHILE,
	"with", WITH
};

const int keywordAmount = sizeof(keywords) / sizeof(keywords[0]);

int main(int argc, char *argv[]){
	Tokenizer *tokenizer = tokenizer_new(argv[1]);

	Token *token;

	clock_t startTime = clock();

	int count = 0;
	while((token = tokenizer_nexttoken(tokenizer)) != NULL){ //End of file
		//printf("%.2d - %s\n", token->type, token->value);
		free(token->value);
		free(token);
		++count;
	}

	printf("Read %d tokens!\n", count);

	tokenizer_destroy(tokenizer);

	setupGrammar();

	Node *node = parseFile(argv[1]);

	printf("%d\n", node->type);

	deleteGrammar();
	printf("Took %f seconds\n", ((double)clock() - startTime) / CLOCKS_PER_SEC);

	return 0;
}

int token_is_word(char *s);
int token_is_number(char *s);
int token_is_string(char *s);
int token_is_symbol(char *s, enum TokenType *t);
enum TokenType token_get_wordtype(char *s);


Tokenizer *tokenizer_new(char *filepath){
	Tokenizer *tokenizer = malloc(sizeof(Tokenizer));

	tokenizer->filepath = malloc(sizeof(filepath));
	strcpy(tokenizer->filepath, filepath); //Copies filepath

	tokenizer->file = fopen(filepath, "r");

	tokenizer->buffer[0] == '\0';
	tokenizer->cur = tokenizer->buffer;
	tokenizer->line = 0;

	return tokenizer;
}

void tokenizer_setfile(Tokenizer *self, char *filepath){
	free(self->filepath);
	self->filepath = malloc(sizeof(filepath));
	strcpy(self->filepath, filepath);

	fclose(self->file);
	self->file = fopen(filepath, "r");

	self->line = 0;
}

void tokenizer_destroy(Tokenizer *self){
	free(self->filepath);
	fclose(self->file);
	free(self);
}

void tokenizer_nextline(Tokenizer *self){
	if(fgets(self->buffer, MAX_SOURCE_LINE + 1, self->file) != NULL){
		self->cur = self->buffer;

	}else{
		self->cur = NULL;
	}

	++(self->line);
}

Token *tokenizer_nexttoken(Tokenizer *self){
	Token *token = malloc(sizeof(Token));

	begin:

	if(self->cur == NULL){
		free(token);
		return NULL;
	}

	while(isspace(*(self->cur))){
		++(self->cur);
	}



	if(*(self->cur) == '\n' || *(self->cur) == '\r' || !*(self->cur) || (*(self->cur) == '/' && *(self->cur + 1) == '/')){
		tokenizer_nextline(self);

		if(self->cur == NULL){
			free(token);
			return NULL;
		}

		goto begin;
	}

	int toklength = 0;
	if(toklength = token_is_word(self->cur)) {
		token->value = malloc(toklength + 1); //+1 for NULL character
		readChars(token->value, self->cur, toklength);
		token->type = token_get_wordtype(token->value);
	}
	else if(toklength = token_is_number(self->cur)) {
		token->value = malloc(toklength + 1); //+1 for NULL character
		token->type = NUMBER;
		readChars(token->value, self->cur, toklength);
	}
	else if(toklength = token_is_string(self->cur)){
		token->value = malloc(toklength - 1); //+1 for NULL character and -2 for quotation marks
		token->type = STRING_CONST;
		readChars(token->value, self->cur + 1, toklength - 2); //Skips quotation marks
	}
	else if(toklength = token_is_symbol(self->cur, &token->type)){
		token->value = malloc(toklength + 1); //+1 for NULL character
		readChars(token->value, self->cur, toklength);
	}
	else{
		toklength = 1;

		token->type = UNDEFINED;
		token->value = malloc(toklength + 1);
		readChars(token->value, self->cur, toklength + 1);

		fprintf(stderr, "Did not recognise token %s at %d:%d in file %s\n",
		token->value, self->line, TOKEN_GET_COLUMN(self), self->filepath);
	}

	self->cur += toklength;
	self->curtok = token;

	return token;
}

int token_is_word(char *s){
	int count = 1;
	if(!IS_WORD_BEGIN(*s))
		return 0;
	++s;
	while(IS_WORD_END(*s)){
		++count;
		++s;
	}
	return count;
}

int token_is_number(char *s){
	int count = 0;
	if(*s == '-' && isdigit(*(s+1))){
		++count;
		++s;
	}
	while(isdigit(*s)){
		++count;
		++s;
	}
	if(*s == '.' && isdigit(*(s+1))){
		++count;
		++s;
		while(isdigit(*s)){
			++count;
			++s;
		}
	}
	if(*s == 'E'){
		++count;
		++s;

		if(*s == '-'){
			++count;
			++s;
		}
		while(isdigit(*s)){
			++count;
			++s;
		}
	}
	return count;
}

int token_is_string(char *s){
	int count = 2;
	if(*s++ != '"')
		return 0;
	while(*s++ != '"')
		++count;
	return count;
}

int token_is_symbol(char *s, enum TokenType *t){
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
		case '.': *t = DOT;			break;
		case ';': *t = SEMICOLON;	break;
		case '#': *t = HASH;		break;
		default:					return 0;
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
				case EXCL: *t = EXCL_EQUALS;		return 2;
				case EQUALS: *t = EQUALS_EQUALS; 	return 2;
				default: 							return 1;
			}
		case '&':
			switch(*t){
				case AMPER: *t = AMPER_AMPER;		return 2;
				default: 							return 1;
			}
		case '|':
			switch(*t){
				case VBAR: *t = VBAR_VBAR;			return 2;
				default: 							return 1;
			}
		case '+':
			switch(*t){
				case PLUS: *t = PLUS_PLUS;			return 2;
				default:							return 1;
			}
		case '-':
			switch(*t){
				case MINUS: *t = MINUS_MINUS;		return 2;
				default:							return 1;
			}
		case '>':
			switch(*t){
				case MINUS:	*t = MINUS_GREATER;		return 2;
				default:							return 1;
			}
	}

	return *t ? 1 : 0;

}

enum TokenType token_get_wordtype(char *s){
	int low = 0;
	int max = keywordAmount - 1;
	int half = max / 2;

	while(low < half && max > half){
		int cmp = strcmp((keywords[half]).key, s);

		if(cmp > 0)
			max = half;
		else if(cmp < 0)
			low = half;
		else
			return keywords[half].type;

		half = (low + max) / 2;
	}

	return IDENTIFIER;
}

char *readChars(char *s, char *t, int num){
	while(num--)
		*s++ = *t++;
	*s = '\0';
	return t;
}


/*int main(int argc, char *argv[]){
	clock_t start = clock();
	//setlocale(LC_CTYPE,"it_IT.UTF-8");

	Token *list = lexFile(argv[1]);
	clock_t tokenTime = clock();
	printf("It took %f seconds to tokenize\n", ((double)(tokenTime - start) / CLOCKS_PER_SEC));

	Token *curToken;

	while((curToken = list) != NULL){
		list = list->next;
		if(curToken->type != UNDEFINED)
			free(curToken->value);
		free(curToken);
	}

	clock_t preGrammarTime = clock();

	setupGrammar();

	clock_t grammarTime = clock();

	printf("It took %f seconds to set up grammar\n", ((double)(grammarTime - preGrammarTime) / CLOCKS_PER_SEC));

	deleteGrammar();

	printf("That is %f seconds in total\n", ((double)clock() - start) / CLOCKS_PER_SEC);

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

			if(*line_ptr == '\n' || *line_ptr == '\r' || !*line_ptr || (*line_ptr == '/' && *(line_ptr + 1) == '/')){
				break;
			}

			Token *token = malloc(sizeof(Token));
			int length = getToken(line_ptr, token);

			//printf("%s\t%d\t%ld\t%d\n", token->value, token->type, strlen(token->value), *token->value); PRINTING OVER HEREE

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
	int length;

	if(length = isWord(s)){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		readBytes(s, token->value, length);
		token->type = getKeywordType(token->value);
		return length;
	}
	else if(length = isNumber(s)){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		readBytes(s, token->value, length);
		token->type = NUMBER;
		return length;
	}
	// else if(*s == '#'){
	// 	token->value = (char *)malloc(length + 1); //Add one for the null character
	// 	readBytes(s, token->value, length);
	// 	token->type = HASH;
	// }
	else if(length = isSymbols(s, &(token->type))){
		token->value = (char *)malloc(length + 1); //Add one for the null character
		readBytes(s, token->value, length);
		return length;
	}
	else if(length = isString(s)){
		token->value = (char *)malloc(length - 1); //-2 for " and ", then + 1 for the null character
		readBytes(s + 1, token->value, length - 2);
		token->type = STRING_CONST;
		return length;
	}
	else{
		token->type = UNDEFINED;
		printf("UNDEFINED!\n");
	}

	return 1;
}

char *readBytes(char *s, char *t, int num){
	while(num--)
		*t++ = *s++;
	*t = '\0';
	return t;
}

int isWord(char *s){
	int count = 0;
	if(*s != '_' && !isalpha(*s)){
		return 0;
	}
	while(*s == '_' || isalnum(*s)){
		++count;
		++s;
	}
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
		case '#': *t = HASH;		break;
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
		case '|':
			switch(*t){
				case VBAR: *t = VBAR_VBAR;		return 2;
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
	int low = 0;
	int max = keywordAmount - 1;
	int half = max / 2;
	int cmp;

	while(low < half && max > half){
		cmp = strcmp((keywords[half]).key, s);

		if(cmp > 0)
			max = half;
		else if(cmp < 0)
			low = half;
		else
			return keywords[half].type;

		half = (low + max) / 2;
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
}*/
