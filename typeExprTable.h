#include "parser.h"



typedef struct{
    int top;
    unsigned capacity;
    parseTree* arr;
} stack;

typedef enum {
    hasDivOp, isLHSReal, isAllBoolVar, hasAllBoolOp, hasRHSreal
} assignment_flags;
// id [ x y z ]
typedef struct {
    typeExpression* varType;
    char* varName;
    int* rangeToFound;
    int rangeNums;
} assignExpression;

typedef struct {
    int linenum;
    assignExpression* lhs;
    assignExpression* lRHS;
    assignExpression* rRHS;
    bool hasDivop,hasBoolop,hasMDAop,hasASOop[2];
} assignment_type_checker;

stack* create_stack();

void push(stack* s,parseTree r);

parseTree pop(stack *s);

void printTypeExpressionTable (typeExpressionTable T);
