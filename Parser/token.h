#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#define IS_SPACE(c)	(c == ' ' || c == '\t' ||\
					 c == '\n'|| c == '\v' ||\
					 c == '\f'|| c == '\r')

#define IS_NUM(c)	(c >= 0 && c <= 9)

#define IS_ALPHA(c) (c >= 'A' && c <= 'Z') ||\
					(c >= 'a' && c <= 'z') ||\
					(c == '_')

#define IS_ALNUM(c) (IS_ALPHA(c)) || (IS_NUM(c))

extern char *keywords[];

enum token_type {IF, WHILE, L_PAREN, R_PAREN, IDENTIFIER};

struct token{
	enum token_type type;
	char *value;
};

struct token_list{
    struct token *tokens;
    size_t size;
    size_t next;
};

typedef struct token Token;
typedef struct token_list Token_List;

void new_list(Token_List *list, size_t init_size);
void add_entry(Token_List *list, Token token);
#endif