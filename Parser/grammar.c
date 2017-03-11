#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "grammar.h"
#include "../General/hashtable.h"

#ifndef MAX_LINE
#define MAX_LINE 1024
#endif

GrammarHead *grammar;
HashTable *grammarTable;

void setupGrammar(){
	FILE *file = fopen("Grammar/grammar.txt", "r");

	grammarTable = table_new(20);
	addToTable(file);

	table_free(grammarTable);

	fclose(file);
}

void addToTable(FILE *file){
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

		line_ptr += length + 1;

		length = getGrammarPart(line_ptr);
		grammarPart = malloc(length + 1);
		readBytes(line_ptr, grammarPart, length);

		printf("%s - %s\n", namePart, grammarPart);

		insert(grammarTable, entry_create(namePart, grammarPart));

		printf("%s\n", (char *)lookup(grammarTable, "expr")->value);

	}
}

int getNamePart(char *line){
	int length = 0;

	while(*line++ != ':')
		++length;

	printf("%d\n", length);

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

	printf("%d\n", length);

	return length;
}