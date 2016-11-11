#include <stddef.h>
#define MAX_TOKEN_VALUE 32

enum token_type {IF, WHILE, L_PAREN, R_PAREN, IDENTIFIER};

char *keywords[] = {"for", //USE A HEADER GUARD
                    "if",
                    "while"};

struct token{
	enum token_type type;
	char *value;
	struct token *next;
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
    