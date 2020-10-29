#include "parser.h"



typedef struct{
    int top;
    unsigned capacity;
    parseTree** arr;
} stack;

typedef enum {
    hasDivOp, isLHSReal, isAllBoolVar, hasAllBoolOp, hasRHSreal
} assignment_flags;
typedef enum {
        ASSIGNMENT, DECLARE
    } statement_type;
// id [ x y z ]
typedef struct {
    typeExpression* varType;
    char* varName;
    int* rangeToFound;
    int rangeNums;
} assignExpression;

typedef struct {
    char* lexeme;
} declare_error;
typedef struct {
    char* firstLexeme;
    char* firstType;
    char* secondLexeme;
    char* secondType;
    char* operator;
} assign_error;

typedef struct {
    statement_type statementType;
    int linenum;
    int depth;
    int errorType;
    char* errorMsg;
    union 
    {
        declare_error declareError;
        assign_error assignError;
    };
} type_error;

type_error* typeError;
// 0 -> var not found
// 1 -> error data type
// 2 -> type not match
// 3 -> dim not match
// 4 -> operator mismatch
// 5 -> 2d sub range not match
// 6 -> 3d sub range not match
// 7 -> 3d sub sub range not match
// 8 -> Index Out of Bounds
// 9 -> extra sub elem in 2d

typedef struct {
    int linenum;
    assignExpression* lhs;
    assignExpression* lRHS;
    assignExpression* rRHS;
    bool hasDivop,hasBoolop,hasMDAop,hasASOop[2],isAddop;
} assignment_type_checker;

stack* create_stack();

void push(stack* s,parseTree* r);

parseTree* pop(stack *s);

void printTypeExpressionTable (typeExpressionTable T);
void printErrorType(type_error*);
