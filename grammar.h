#include "lexer.h"
#include<stdbool.h>

#define  NO_OF_RULES 100

struct RHS_NODE{
    union{
        terminal term;
        non_terminal non_term;
    };
    bool is_terminal;
    struct RHS_NODE *next;
};
typedef struct RHS_NODE rhs_node;


typedef struct{
    non_terminal lhs;
    rhs_node* first_rhs;
} cell_node;

cell_node* grammar;

void readGrammar(char* filename,cell_node *grammar);




