#ifndef GROUPING_H
#define GROUPING_H

enum NodeType {VALUE, EXPR, STMT, ASSIGN};

typedef struct node{
    enum NodeType;
} Node;

typedef struct value_node{
    //Value
} ValueNode;

typedef struct expr_node{
    ValueNode value;
    //operator
    //expr1, expr2
} ExprNode;

typedef struct stmt_node{

} StmtNode;

typedef struct assign_node{

} AssignNode;


#endif
