#ifndef PARSER_H
#define PARSER_H

/*enum NodeType {EXPRNODE, ARITHNODE, ASSIGNNODE, };

enum Operators {ADD, SUBTRACT, MULTIPLY, DIVIDE, MODULO, POWER};

struct node;
typedef struct node Node;


struct programnode;
typedef struct programnode ProgramNode;


struct exprnode;
typedef struct exprnode ExprNode;

struct arithnode;
typedef struct arithnode ExprNode;

struct identifiernode;
typedef struct identifiernode IdentifierNode;

struct numbernode;
typedef struct numbernode NumberNode;

struct stringnode;
typedef struct stringnode StringNode;


struct stmtnode;
typedef struct stmtnode StmtNode;

struct assignnode;
typedef struct assignnode AssignNode;


struct node{
    enum NodeType type;
};

struct programnode{
    Node node;
};

struct exprnode{ //Anything which returns a value
    Node node;

    enum NodeType exprtype;
    union expr{             //Should these be pointers or not? Not, I think, makes
        ArithNode arith;    //more sense, as an expression would then be one node,
        IdentifierNode identifier;//and not two
        NumberNode number;
        StringNode string;
        //MatchNode match;
        //IfNode if;
    }
};

struct arithnode{
    Node node;
    enum Operators op;
    struct ExprNode *operand1;
    struct ExprNode *operand2;
};

struct identifiernode{
    Node node;
    char *name;
};

struct numbernode{
    Node node;
    char *number;
};

struct stringnode{
    Node node;
    char *string;
};

struct stmtnode{
    Node node;
};

struct assignnode{
    StmtNode stmt;
    IdentifierNode identifier;
    ExprNode expr;
};*/

struct node;
typedef struct node Node;

struct prognode;
typedef struct prognode ProgNode;

struct assignnode;
typedef struct assignnode AssignNode;

struct returnnode;
typedef struct returnnode ReturnNode;

struct exprnode;
typedef struct exprnode ExprNode;

enum NodeType {PROG, ASSIGNSTMT, RETURNSTMT, EXPR};

struct node{
    enum NodeType type;
};

struct prognode{
    Node node;

    Node **stmts;
};

struct assignnode{
    Node node;

    Token *identifier;
    ExprNode *expr;
};

struct returnnode{
    Node node;

    ExprNode *expr;
};

struct exprnode{
    Node node;

    Token *expression;
};

Node *buildProg(size_t start);
Node *buildAssign(size_t start);
Node *buildReturn(size_t start);
Node *buildExpr(size_t start);


Node *parseFile(char *filepath);

#endif
