#include "token.h"
#include <stddef.h>
#include <stdlib.h>

struct token_list{
    Token *tokens;
    size_t size;
    size_t next;
}

void new_list(Token_List *list){
    list = (Token_List *)malloc(sizeof(Token_List));
    list->tokens = malloc(sizeof(Token));
    list->size = 1;
    list->next = 0;
}

void add_entry
