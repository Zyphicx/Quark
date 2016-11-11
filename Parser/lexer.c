#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#define MAX_LINE 512

void lex(FILE *file, Token_List *list);
Token *get_next_token(char *line);
int get_next_word(char *s, char *target, int max);

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
	while(1){
		if(fgets(line, MAX_LINE - 1, file) == NULL){
			fprintf(stderr, "Error with reading line");
			exit(1);
		}
		/*printf("%s\n", line);
		get_next_token(line);*/

	}
}

Token *get_next_token(char *line){
	Token token;
	token.value = malloc(MAX_TOKEN_VALUE * sizeof(char));

	//Check for words
	if(get_next_word(line, token.value, MAX_TOKEN_VALUE)){
		printf("%s", token.value);
	}

	//Check for numbers

	//Check for specific characters
}

int get_next_word(char *s, char *target, int max){ //USE STRTOK OTHERWISE!!!!!!!!
	while(isspace(*s++))
		;
	int count = 1;
	if(!isalpha(*target++ = *s++))
		return 0;
	while(isalnum(*target++ = *s++) && --max > 1)
		count++;
	*target = '\0';
	return count + 1;
}