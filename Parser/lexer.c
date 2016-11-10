#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#define MAX_LINE 512

void lex(FILE *file, Token_List *list);
Token *get_next_token(char *line);
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
	while(1){
		if(fgets(line, MAX_LINE - 1, file) == NULL){
			fprintf(stderr, "Error with reading line");
			exit(1);
		}
		printf("%s\n", line);
		get_next_token(line);
	}
	/*if(str_contains(file, "for")){
		printf("It does contain for");
	}*/
}

Token *get_next_token(char *line){
	Token token;

	if(str_contains(line, "for"))
		token.value = 
}

int str_contains(char *s, char *val){
	while(*s == ' ' || *s == '\t')
		++s;

	while(*s == *val && *val){
		s++; 
		val++;
	}
        s = 
	return *val ? 0 : 1;
}

void str_cut(char **str, int amount){
        while(amount-- > 0){
             (*str)++;
        }
}
