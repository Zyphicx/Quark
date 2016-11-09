#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#define MAX_LINE 512

void lex(FILE *file, Token_List *list);
Token *get_next_token(FILE *file);
void lex(FILE *file, Token_List *list);
int str_contains(char *s, char *val);

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
	fgets(line, MAX_LINE - 1, file);
	printf("%s\n", line);
	/*if(str_contains(file, "for")){
		printf("It does contain for");
	}*/
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
	while(*s == *val && *val){
		s++; 
		val++;
	}
	return *val ? 0 : 1;
}