#include "lexer.h"

#define  NO_OF_RULES 100

typedef struct{
    union{
        token t;
        non_terminal nt;
    };
    bool is_terminal;
    rhs_node *next;
} rhs_node;


typedef struct{
    non_terminal lhs;
    rhs_node first_rhs;
} cell_node;

cell_node grammar[NO_OF_RULES];

FILE *fp;
fp = fopen("grammar.txt", "r");

void readGrammar(FILE *fp,cell_node *grammar);




