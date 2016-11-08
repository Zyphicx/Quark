#define MAX_TOKEN_VALUE 32

enum token_type {IF, WHILE, L_PAREN, R_PAREN, IDENTIFIER};

typedef struct token Token;
typedef struct token_list Token_List;

struct token{
	enum token_type type;
	char *value;
	Token *next;
};

struct token_list;

void List_New(Token_List *list){
	list = (Token_List *)malloc(sizeof(Token_List));
	list->size = 1;
	list->next = 0;
}

void List_Add(Token_List *list, Token token){
	if(list->used >= list->size){
		list = realloc()
	}
}
