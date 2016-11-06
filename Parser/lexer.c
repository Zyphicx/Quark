#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKEN_VALUE 32

enum token_type {IF, WHILE, L_PAREN, R_PAREN, IDENTIFIER};

typedef struct token Token;

struct token{
	enum token_type type;
	char value[MAX_TOKEN_VALUE];
	Token *next;
};

int main(){
	printf("%d\n", str_contains("Heyo", "Heyoa"));
}

void lex(FILE *file, Token *list){

}

Token *get_next_token(FILE *file){
	register char c;
	Token *token = malloc(sizeof(Token));

	while((c = getchar()) != EOF){
	}
	if(c == EOF){
		free(token);
	}
}

int str_contains(char *s, char *val){
	while(*s++ == *val++ && *val)
		;
	return *val ? 0 : 1;
}