#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

enum TokenType {NUMBER, IDENTIFIER, STRING_CONST, INTEGER, DOUBLE, LIST, STRING, IF, INCLUDE, ELSE, FOR, WHILE, 
				PLUS, MINUS, MULTIPLY, STAR, SLASH, PERCENT, CARET, GREATER, LESS, EXCL, QUESTION, EQUALS, AMPER, VBAR, 
				L_PAREN, R_PAREN, L_CURLY, R_CURLY, L_SQUARE, R_SQUARE, COMMA, COLON, SEMICOLON, PLUS_EQUALS, MINUS_EQUALS, 
				STAR_EQUALS, SLASH_EQUALS, PERCENT_EQUALS, CARET_EQUALS, GREATER_EQUALS, LESS_EQUALS, EXCL_EQUALS,
				EQUALS_EQUALS, AMPER_AMPER, PLUS_PLUS, MINUS_MINUS, PREPROCESSOR, UNDEFINED};

typedef struct{
	const char *key;
	const enum TokenType type;
} Keyword;

extern Keyword keywords[];
extern const int keywordAmount;

typedef struct token{
	enum TokenType type;
	char *value;
	struct token *next;
} Token;

Token *lexFile(char *path);
int getToken(char *s, Token *token);
char *readBytes(char *s, char *t, int num);
int isWord(char *s);
int isNumber(char *s);
int isPreprocessor(char *s);
int isSymbols(char *s, enum TokenType *type);
enum TokenType getKeywordType(char *s);
int strEquals(const char *s, const char *t);

#endif