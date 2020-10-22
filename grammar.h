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
    parse_node* parent;
    int currchildren = 0;
    int size = grammarrulesize
    parse_node* children[];


} parse_node;
typedef struct{
    non_terminal lhs;
    rhs_node* first_rhs;
    rhs_node* last_rhs;
} cell_node;


struct Grammar{
   cell_node* grammar_rules;
};

typedef struct Grammar grammar;


void readGrammar(char* filename,cell_node *grammar);




