#include "grammar.h"




typedef struct{
    int top;
    unsigned capacity;
    rhs_node* arr;
} stack;


stack* create_stack();

void push(stack* s,rhs_node r);

rhs_node pop(stack *s);


void createParseTree( parseTree *t, tokenStream *s, grammar G);

