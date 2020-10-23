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
    struct RHS_NODE *prev;
};
typedef struct RHS_NODE rhs_node;



typedef struct{
    non_terminal lhs;
    rhs_node* first_rhs;
    rhs_node* last_rhs;
} cell_node;


struct Grammar{
   cell_node* grammar_rules;
};

typedef struct Grammar grammar;

typedef struct parseTree{
    struct parseTree* parent;
    struct parseTree * left_most_child;
    struct parseTree* right_most_child;
    struct parseTree* right_sibling;
    union 
    {
        terminal term;
        non_terminal non_term;
    };
    char lexeme [MAX_VAR_NAME_LEN];
    bool is_terminal;
} parseTree;

void readGrammar(char* filename,cell_node *grammar);
int** hardCodedRules();
int** initialiseRules(int **rules);




