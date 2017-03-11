#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "../Parser/token.h"

typedef struct grammar_part GrammarPart;
typedef struct grammar_head GrammarHead;

struct grammar_part{
	enum TokenType type;
	struct GrammarPart **next;
};

struct grammar_head{
	char *name;
	struct GrammarPart **next;
	struct GrammarHead *nextGrammar;
};

void setupGrammar();
void addToTable();
int getNamePart(char *line);
int getGrammarPart(char *line);

#endif