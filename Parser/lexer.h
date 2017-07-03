#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "stdio.h"

#define MAX_SOURCE_LINE 4096

#define TOKEN_GET_COLUMN(self) (self->cur - self->buffer + 1)

#define IS_WORD_BEGIN(c)    (c == '_' || (c >= 'a' && c <= 'z') ||\
                            (c >= 'A' && c <= 'Z'))

#define IS_WORD_END(c)      (IS_WORD_BEGIN(c) || (c >= '0' && c <= '9'))

extern TokenName keywords[];
extern const int keywordAmount;

typedef struct tokenizer{
    char *filepath;
    FILE *file;

    char buffer[MAX_SOURCE_LINE];
    char *cur;
    size_t line;

    Token *curtok;
} Tokenizer;

Tokenizer *tokenizer_new(char *filepath);
void tokenizer_setfile(Tokenizer *self, char *filepath);
void tokenizer_destroy(Tokenizer *self);
Token *tokenizer_nexttoken(Tokenizer *self);
char *readChars(char *s, char *t, int num);

/*Token *lexFile(char *path);
int getToken(char *s, Token *token);
char *readBytes(char *s, char *t, int num); //NO NEED TO HAVE ALL OF THESE PUBLIC
int isWord(char *s);
int isNumber(char *s);
int isPreprocessor(char *s);
int isSymbols(char *s, enum TokenType *type);
int isString(char *s);
enum TokenType getKeywordType(char *s);
int strEquals(const char *s, const char *t);*/

#endif
