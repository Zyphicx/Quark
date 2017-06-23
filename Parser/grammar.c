#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "grammar.h"

#define MAX_LINE 4096

//void addToTable(FILE *file);
void createGrammars(FILE *file);
void deleteGrammar();
void freeGrammarPart(GrammarPart *part);
GrammarPart *createGrammar(char **line);
GrammarPart *getNextPart(char **line);
int getNamePart(char *line);
int getGrammarPart(char *line);
enum TokenType getTokenType(char *s);

Keyword tokenNames[] = {
	"NUMBER", NUMBER,
	"IDENTIFIER", IDENTIFIER,
	"STRING_CONST", STRING_CONST,
	"DOUBLE", DOUBLE,
	"ELSE", ELSE,
	"IF", IF,
	"INCLUDE", INCLUDE,
	"INTEGER", INTEGER,
	"FOR", FOR,
	"LIST", LIST,
	"RETURN", RETURN,
	"STRING", STRING,
	"TUPLE", TUPLE,
	"WHILE", WHILE,
	"PLUS", PLUS,
	"MINUS", MINUS,
	"STAR", STAR,
	"SLASH", SLASH,
	"PERCENT", PERCENT,
	"CARET", CARET,
	"GREATER", GREATER,
	"LESS", LESS,
	"EXCL", EXCL,
	"QUESTION", QUESTION,
	"EQUALS", EQUALS,
	"AMPER", AMPER,
	"VBAR", VBAR,
	"L_PAREN", L_PAREN,
	"R_PAREN", R_PAREN,
	"L_CURLY", L_CURLY,
	"R_CURLY", R_CURLY,
	"L_SQUARE", L_SQUARE,
	"R_SQUARE", R_SQUARE,
	"COMMA", COMMA,
	"COLON", COLON,
	"SEMICOLON", SEMICOLON,
	"PLUS_EQUALS", PLUS_EQUALS,
	"MINUS_EQUALS", MINUS_EQUALS,
	"STAR_EQUALS", STAR_EQUALS,
	"SLASH_EQUALS", SLASH_EQUALS,
	"PERCENT_EQUALS", PERCENT_EQUALS,
	"CARET_EQUALS", CARET_EQUALS,
	"GREATER_EQUALS", GREATER_EQUALS,
	"LESS_EQUALS", LESS_EQUALS,
	"EXCL_EQUALS", EXCL_EQUALS,
	"EQUALS_EQUALS", EQUALS_EQUALS,
	"AMPER_AMPER", AMPER_AMPER,
	"VBAR_VBAR", VBAR_VBAR,
	"PLUS_PLUS", PLUS_PLUS,
	"MINUS_MINUS", MINUS_MINUS
};

const int tokenNameAmount = sizeof(tokenNames) / sizeof(tokenNames[0]);

GrammarHead *grammar;
HashTable *grammarTable;

void setupGrammar(){
	FILE *file = fopen("Grammar/grammar.qgf", "r");

	grammarTable = table_new(20);
	//addToTable(file);

	//rewind(file); //Makes file ready for another read

	createGrammars(file);

	deleteGrammar();

	fclose(file);
}

void deleteGrammar(){

	GrammarHead *h = grammar;
	while(h != NULL){
		GrammarHead *nextHead = h->nextGrammar;
		GrammarPart *g = h->next;

		while(g != NULL){
			GrammarPart *nextPart = g->next;

			printf("%d", g->type);
			switch(g->type){
				case OR:
					printf("(OR) -> ");
					break;
				case OPTIONAL:
					printf("(OPTIONAL) -> ");
					break;
				case LOOP:
					printf("(LOOP) -> ");
					break;
				case MACRO:
					printf("(MACRO) -> ");
					break;
				default:
					printf(" -> ");
					break;
			}

			g = nextPart;
		}

		printf("\n");

		freeGrammarPart(h->next);

		free(h->name);
		free(h);

		h = nextHead;
	}

	table_free(grammarTable);
}

void freeGrammarPart(GrammarPart *part){
	GrammarPart *p = part;
	while(p != NULL){
		GrammarPart *next = p->next;

		switch(p->type){
			case OPTIONAL:
				freeGrammarPart(((OpGrammarPart *)p)->option);
				break;
			case OR:
				freeGrammarPart(((OrGrammarPart *)p)->op1);
				freeGrammarPart(((OrGrammarPart *)p)->op2);
				break;
			case LOOP:
				freeGrammarPart(((LoopGrammarPart *)p)->loop);
				break;
			case MACRO:
				free(((MacroGrammarPart *)p)->macro);
				break;
		}

		free(p);
		p = next;
	}
}

/*
	addToTable adds all grammars into a hashtable as strings.
  This is what makes it possibe for one grammar to reference
	another one using its name, just like a macro!
*/
/*void addToTable(FILE *file){
	char line[MAX_LINE];

	while(fgets(line, MAX_LINE + 1, file) != NULL){
		if(*line == '\n' || *line == '\r')
			continue;


		char *line_ptr = line;
		char *namePart;
		char *grammarPart;

		int length;

		length = getNamePart(line_ptr);
		namePart = malloc(length + 1);
		readBytes(line_ptr, namePart, length);

		line_ptr += length + 1; //Skips the colon

		length = getGrammarPart(line_ptr);
		grammarPart = malloc(length + 1);
		readBytes(line_ptr, grammarPart, length);

		//printf("%s - %s\n", namePart, grammarPart);

		table_insert(grammarTable, entry_create(namePart, grammarPart));

		//printf("%s\n", (char *)table_lookup(grammarTable, namePart)->value);

		free(namePart); //namePart is being COPIED by the insert function, the actual namePart pointer
										//is not being saved there itself. This is for compatibility with string literals.
	}
}*/

void createGrammars(FILE *file){
	char line[MAX_LINE];
	GrammarHead *grammars = NULL;
	GrammarHead *curHead = NULL;

	while(fgets(line, MAX_LINE + 1, file) != NULL){
		if(*line == '\n' || *line == '\r' || *line == '#')
			continue;

		char *line_ptr = line;

		GrammarHead *head = malloc(sizeof(GrammarHead));

		int nameLength = getNamePart(line_ptr);
		head->name = malloc(nameLength + 1);
		readBytes(line_ptr, head->name, nameLength);

		head->nextGrammar = NULL;

		printf("%s\n", head->name);

		line_ptr += nameLength + 1; //Skips the colon

		if(grammars == NULL){
			grammars = head;
			curHead = head;
		}
		else{
			curHead->nextGrammar = head;
			curHead = head;
		}

		head->next = createGrammar(&line_ptr);

		table_insert(grammarTable, entry_create(head->name, head->next));
	}

	grammar = grammars; //Sets global grammar to local grammar
}

GrammarPart *createGrammar(char **line){
	GrammarPart *g = NULL;
	GrammarPart *curPart = NULL;

	while(**line && **line != '\n' && **line != '\r' && **line != ')'){
		GrammarPart *part = getNextPart(line);

		//sleep(3);

		if(g == NULL){
			g = part;
			curPart = part;
		}
		else{
			GrammarPart *p;
			for(p = curPart; p->next != NULL; p = p->next) 	//Walks list to find end
				;											//To then attach part

			p->next = part;
			curPart = part;
		}
	}

	return g;
}

GrammarPart *getNextPart(char **line){
	char token[MAX_LINE + 1];
	char *ptr = token;

	GrammarPart *part;
	GrammarPart *tokenPart;

	while(isspace(**line))
		++(*line);

	if(isupper(**line)){ //If character is upper, then it's a token, read token
		while(isupper(**line) || **line == '_'){
			*ptr++ = *(*line)++;
		}
		*ptr = '\0';

		tokenPart = malloc(sizeof(GrammarPart));

		tokenPart->type = getTokenType(token);
		tokenPart->next = NULL;

		printf("Still running! %d\n", tokenPart->type);

	}else if(islower(**line)){ //If character is lower, then it's a macro, expand
		int chars = 1; //Add 1 for the NULL character at the end
		while(islower(**line) || **line == '_'){
			*ptr++ = *(*line)++;
			++chars;
		}
		*ptr = '\0';

		tokenPart = malloc(sizeof(MacroGrammarPart));

		tokenPart->type = MACRO;
		tokenPart->next = NULL;

		((MacroGrammarPart *)tokenPart)->macro = malloc(chars * sizeof(char));
		strcpy(((MacroGrammarPart *)tokenPart)->macro, token);

		printf("Still running! %d (Macro): %s\n", tokenPart->type, ((MacroGrammarPart *)tokenPart)->macro);

		//printf("Still running! %s\n", table_lookup(grammarTable, token)->value);

		/*ptr = token;

		strcpy(ptr, table_lookup(grammarTable, token)->value);

		tokenPart = createGrammar(&ptr);*/

	}else if(**line == '('){ //If character is parenthesis, then it's a group, read group
		++(*line); //Skips the opening parenthesis
		tokenPart = createGrammar(line);
		++(*line); //Skips the ending parenthesis
	}else{
		return NULL;
	}

	while(isspace(**line)) //Skips spaces between tokens and operators
		++(*line);

	if(**line == '-'){
		++(*line);

		part = malloc(sizeof(OpGrammarPart));
		part->type = OPTIONAL;
		part->next = NULL;

		((OpGrammarPart *)part)->option = tokenPart;

	}else if(**line == '|'){
		++(*line);

		part = malloc(sizeof(OrGrammarPart));
		part->type = OR;
		part->next = NULL;

		((OrGrammarPart *)part)->op1 = tokenPart;
		((OrGrammarPart *)part)->op2 = getNextPart(line);

	}else if(**line == '*'){
		++(*line);

		part = malloc(sizeof(LoopGrammarPart));
		part->type = LOOP;
		part->next = NULL;

		((LoopGrammarPart *)part)->loop = tokenPart;

	}else{
		part = tokenPart;
	}

	while(isspace(**line))
		++(*line);

	return part;
}

int getNamePart(char *line){
	int length = 0;

	while(*line++ != ':')
		++length;

	return length;
}

int getGrammarPart(char *line){
	int length = 0;

	while(isspace(*line)){
		++length;
		++line;
	}

	while(*line != '\n' && *line != '\r' && *line){
		++length;
		++line;
	}

	return length;
}

enum TokenType getTokenType(char *s){
	Keyword *kp;
	for(kp = tokenNames; kp < (tokenNames + tokenNameAmount); kp++){
		if(strEquals(s, kp->key)){
			return kp->type;
		}
	}
	return UNDEFINED;
}
