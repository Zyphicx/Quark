#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "token.h"

struct token_list{
	Token *token;
	size_t next;
	size_t size;
}

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