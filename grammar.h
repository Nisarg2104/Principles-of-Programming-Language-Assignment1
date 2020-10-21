#include "lexer.h"


#define  NO_OF_RULES 100

typedef struct{
    union{
        terminal t;
        non_terminal nt;
    };
    bool is_terminal;
    rhs_node *next;
} rhs_node;


typedef struct{
    non_terminal lhs;
    rhs_node* first_rhs;
} cell_node;

cell_node grammar[NO_OF_RULES];

void readGrammar(char* filename,cell_node *grammar);




