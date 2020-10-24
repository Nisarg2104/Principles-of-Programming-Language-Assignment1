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

typedef enum {
    _prim, _array,_jagged,_error
} type;

typedef enum {
    STATIC, DYNAMIC, NA
} array_type;

typedef struct rect_array{
    char* range [2];
    struct rect_array* next;
} rect_array;
typedef struct{
   rect_array* first;
   rect_array* last;
} rect_array_ranges;

typedef struct{
    char** ranges;
    int index;
} tdJArrRange;

typedef struct {
    int* subSubRanges;
    int subRangeCount;
    int index;
}thd_sub_range;

typedef struct {
    thd_sub_range* subRanges;
    int index;
} thdJArrRange;


typedef struct{
    type dataType;
    array_type arrayType;
    char* typeName;
    int dimensions;
    char* range_R1 [2];
    int high;
    int low;
    union {        
        rect_array_ranges* rectArrayRanges;
        tdJArrRange tdjaggedArrayRange;
        thdJArrRange thdJaggedArrayRange;
    };    

} typeExpression;

typedef struct data_type{
    typeExpression* type;
    char* varName;
    struct data_type* next;
} dataType;

typedef struct{
    dataType* firstVariable;
    int variables;
}type_expression_table;

typedef type_expression_table* typeExpressionTable;

struct Grammar{
   cell_node* grammar_rules;
};

typedef struct Grammar grammar;

typedef struct parseTree{
    struct parseTree* parent;
    struct parseTree * left_most_child;
    struct parseTree* right_sibling;
    struct parseTree* currNode;
    typeExpression* type;
    int linenum;
    int rulenum;
    int depth;
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
void traverseParseTree(parseTree *t, typeExpressionTable T);