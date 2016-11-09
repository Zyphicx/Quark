#include "token.h"
#include <stddef.h>
#include <stdlib.h>

struct token_list{
    Token *tokens;
    size_t size;
    size_t next;
}

void new_list(Token_List *list, int init_size){
    list = (Token_List *)malloc(sizeof(Token_List));
    list->tokens = malloc(sizeof(Token));
    list->size = init_size;
    list->next = 0;
}

void add_entry(Token_List *list){
    if(list->next == (list->size - 1))
}
