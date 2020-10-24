#include "parser.h"



typedef struct{
    int top;
    unsigned capacity;
    parseTree* arr;
} stack;


stack* create_stack();

void push(stack* s,parseTree r);

parseTree pop(stack *s);

void printTypeExpressionTable (typeExpressionTable T);
