#include <stdlib.h>
#include "token.h"

#define INCREASE_SIZE 64

void new_list(Token_List *list, size_t init_size){
    list->tokens = (Token *)malloc(init_size * sizeof(Token));
    list->size = init_size;
    list->next = 0;
}

void add_entry(Token_List *list, Token token){
    if(list->next == list->size){
    	list->tokens = (Token *)realloc(list->tokens, list->size + INCREASE_SIZE);
    	list->size = list->size + INCREASE_SIZE;
    }
    list->tokens[list->next++] = token;
}
