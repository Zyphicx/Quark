#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#define IS_SPACE(c)	(c == ' ' || c == '\t' ||\
					 c == '\n'|| c == '\v' ||\
					 c == '\f'|| c == '\r')

#define IS_NUM(c)	(c >= '0' && c <= '9')

#define IS_ALPHA(c) (c >= 'A' && c <= 'Z') ||\
					(c >= 'a' && c <= 'z') ||\
					(c == '_')

#define IS_ALNUM(c) (IS_ALPHA(c)) || (IS_NUM(c))

enum token_type {NUMBER, IDENTIFIER, STRING, ADD, SUBTRACT, MULTIPLY, DIVIDE, MODULO, EXPONENT,
	GREATER, LESS, GREATER_EQUALS, LESS_EQUALS, NOT, NOT_EQUALS, AMPER, AND, VBAR, OR, ASSIGN, ADD_ASSIGN, SUBTRACT_ASSIGN,
	MULTIPLY_ASSIGN, DIVIDE_ASSIGN, MODULO_ASSIGN, EXPONENT_ASSIGN, EQUALS,
	DELETE, IF, INCLUDE, ELIF, ELSE, FOR, WHILE, L_PAREN, R_PAREN, L_CURLY, R_CURLY,
	L_BRACK, R_BRACK, COMMA, COLON, SEMICOLON, PREPROCESSOR, UNDEFINED};

struct key_type_pair{
	const char *key;
	const enum token_type type;
};

extern struct key_type_pair keywords[];
extern const int keywords_amount;

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