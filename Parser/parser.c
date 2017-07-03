#include <stdlib.h>

#include "grammar.h"
#include "parser.h"
#include "lexer.h"
#include "../General/hashtable.h"

#define DEFAULT_OUTPUT_SIZE 500

Node *matchGrammar(GrammarHead *head);
int matchPart(GrammarPart *part);

#define TOKEN 0
#define NODE 1

typedef struct{
    unsigned int type : 1;

    union{
        Token *token;
        Node *node;
    } val;
} OutputValue;

struct{
    OutputValue *values;

    size_t size;
    size_t cur; //Where read from/write to, if equal to top, where to place next token
    size_t top; //Where to place next token

    Tokenizer *tokenizer;
} output;

void output_setup(char *filepath){
    output.values = malloc(DEFAULT_OUTPUT_SIZE * sizeof(OutputValue));

    output.size = DEFAULT_OUTPUT_SIZE;
    output.cur = 0;
    output.top = 0;

    output.tokenizer = tokenizer_new(filepath);
}

void output_reset(){
    free(output.values);

    output.cur = 0;
    output.top = 0;

    tokenizer_destroy(output.tokenizer);
}

OutputValue *output_next(){
    if(output.cur == output.top){ //Read from token stream
        while(output.top >= output.size){
            output.size *= 1.5;
            output.values = realloc(output.values, output.size);
        }

        OutputValue entry;

        entry.type = TOKEN;

        Token *token = tokenizer_nexttoken(output.tokenizer);

        entry.val.token = token;

        output.values[output.top++] = entry;
    }

    return output.values + (output.cur++);
}

Node *parseFile(char *filepath){
    output_setup(filepath);

    Node *prog = matchGrammar(table_lookup(grammarTable, "prog"));

    output_reset();

    return prog;
}

Node *matchGrammar(GrammarHead *head){
    printf("Matching %s!\n", head->name);

    size_t start = output.cur;

    GrammarPart *part;
    for(part = head->grammar; part != NULL; part = part->next){
        if(!matchPart(part))
            return NULL;
    }

    output.top = start + 1;

    if(output.cur > output.top)
        output.cur = output.top;

    return head->builder(start);
}

int matchPart(GrammarPart *part){
    if(part->type == OR){
        size_t start = output.cur;
        OrGrammarPart *orPart = (OrGrammarPart *)part;

        GrammarPart *p;

        int success = 1;
        for(p = orPart->op1; p != NULL; p = p->next){
            if(!(success = matchPart(p)))
                break;
        }

        if(success)
            return 1;

        output.cur = start;

        success = 1;
        for(p = orPart->op2; p != NULL; p = p->next){
            if(!(success = matchPart(p)))
                break;
        }

        if(success)
            return 1;


        return 0;
    }
    else if(part->type == MACRO){
        size_t start = output.cur;
        MacroGrammarPart *macroPart = (MacroGrammarPart *)part;

        OutputValue entry;

        entry.type = NODE;
        entry.val.node = matchGrammar(table_lookup(grammarTable, macroPart->macro));

        if(entry.val.node == NULL)
            return 0;

        output.values[start] = entry;

        output.top = start + 1;
        if(output.cur > start)
            output.cur = start + 1;

        return 1;
    }
    else if(part->type == LOOP){

        LoopGrammarPart *loopPart = (LoopGrammarPart *)part;

        while(1){
            size_t start = output.cur;

            int success = 1;

            GrammarPart *p;
            for(p = loopPart->loop; p != NULL; p = p->next){
                if(!(success = matchPart(p)))
                    break;
            }

            if(!success){
                printf("Loop failed here!\n");
                output.cur = start;
                break;
            }
        }

        return 1;
    }
    else if(part->type == OPTIONAL){
        OpGrammarPart *opPart= (OpGrammarPart *)part;

        size_t start = output.cur;

        int success = 1;

        GrammarPart *p;
        for(p = opPart->option; p != NULL; p = p->next){
            if(!(success = matchPart(p)))
                break;
        }

        if(!success)
            output.cur = start;
    }
    else{
        Token *token = output_next()->val.token;

        if(token == NULL)
            return 0;

        printf("Comparing %d to %d!\n", part->type, token->type);

        return part->type == token->type;
    }
}

Node *buildProg(size_t start){
    ProgNode *prog = malloc(sizeof(ProgNode));
    ((Node *)prog)->type = PROG;

    int i;
    for(i = 0; output.values[start + i].type == NODE; i++)
        ;

    printf("Amount of statements in prog: %d\n", i);

    prog->stmts = malloc(i * sizeof(Node *));

    int j;
    for(j = 0; j < i; j++)
        prog->stmts[j] = output.values[start + j].val.node;

    return (Node *)prog;
}

Node *buildAssign(size_t start){
    AssignNode *assign = malloc(sizeof(AssignNode));
    ((Node *)assign)->type = ASSIGNSTMT;

    assign->identifier = output.values[start].val.token;
    assign->expr = (ExprNode *)output.values[start + 2].val.node; //Skips equals sign

    return (Node *)assign;
}

Node *buildReturn(size_t start){
    ReturnNode *ret = malloc(sizeof(ReturnNode));
    ((Node *)ret)->type = RETURNSTMT;

    ret->expr = (ExprNode *)output.values[start].val.node;

    return (Node *)ret;
}

Node *buildExpr(size_t start){
    ExprNode *expr = malloc(sizeof(ExprNode));
    ((Node *)expr)->type = EXPR;

    expr->expression = output.values[start].val.token;

    return (Node *)expr;
}
