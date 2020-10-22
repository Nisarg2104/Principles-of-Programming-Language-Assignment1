#include "grammar.h"


void readGrammar(char* filename,cell_node *grammar) {
    int linenum = 0;
    FILE* grFile = fopen(filename, "r");
    
    while(!feof(grFile)) {
        char* BUF = malloc(200);
        fgets(BUF,200,grFile);
        char *token;
        token = strtok(BUF, " \t\r\n");

        grammar[linenum].lhs = returnNonTerminalVal(token);
        
        rhs_node* rhs = grammar[linenum].first_rhs;

        // printf("%s -> ", token);

        token = strtok(NULL," \t\r\n");
        token = strtok(NULL," \t\r\n");

        
        while(token != NULL) {

            if(rhs==NULL) {
                grammar[linenum].first_rhs = malloc(sizeof(rhs_node));
                rhs = grammar[linenum].first_rhs;
            }
            else {
                rhs->next = malloc(sizeof(rhs_node));
                rhs->next->prev = rhs;
                rhs = rhs->next;
            }
            
            if(isupper(token[0])) {
                rhs->is_terminal = false;
                rhs->non_term = returnNonTerminalVal(token);
            }
            else
            {
                rhs->is_terminal = true;
                rhs->term = returnTerminalVal(token);
            }

            // printf("%s ",token);
            grammar[linenum].last_rhs = rhs;
            token = strtok(NULL," \t\r\n");            
        }
        // printf("\n");

        rhs_node* curr = grammar[linenum].last_rhs;

        // while(curr!=NULL) {
        //     printf("%d:",curr->is_terminal);
        //     if(curr->is_terminal) {
        //         printf("%d\t",curr->term);
        //     }
        //     else
        //     {
        //         printf("%d\t",curr->non_term);
        //     }

        //     curr = curr->prev;
            
        // }

        // printf("\n");

        linenum++;


    }

    

}
int ** hardCodedRules()
{
    int** p = malloc(39*(sizeof(int*)));
    for(int i=0;i<39;i++) {
        p[i] = malloc(31*sizeof(int));
        memset(p[i], -1, 31*sizeof(int));
    }

    return p;
}
void initialiseRules(int **rules)
{
    rules[PROGRAM][program] = 0;
    rules[STATEMENTS][id] = 1;
    rules[STATEMENTS][declare] = 1;
    rules[STATEMENTS][cb_cl] = 2;
    rules[STATEMENT][declare] = 3;
    rules[STATEMENT][id] = 4;
    rules[DECLARE_STATEMENT][id] = 5;
    rules[DECLARE_STATEMENT][list] = 6;
    rules[SINGLE_DECLARE][id] = 7;
    rules[MULTI_DECLARE][list] = 8;
    rules[TYPE][integer] = 9;
    rules[TYPE][real] = 9;
    rules[TYPE][boolean] = 9;
    rules[TYPE][array] = 10;
    rules[TYPE][jagged] = 10;
    rules[ARRAY][array] = 11;
    rules[ARRAY][jagged] = 12;
    rules[PRIM_TYPE][integer] = 13;
    rules[PRIM_TYPE][boolean] = 14;
    rules[PRIM_TYPE][real] = 15;
    rules[RECTANGULAR][array] = 16;
    rules[JAGGED][jagged] = 17;
    rules[JAGGED_INIT][sq_op] = 18;
    rules[JAGGED_INIT_DASH][of] = 19;
    rules[JAGGED_INIT_DASH][sq_op] = 20;
    rules[TD_INIT][of] = 21;
    rules[THD_INIT][sq_op] = 22;
    rules[TD_VALS][r1] = 23;
    rules[TD_VALS_DASH][r1] = 24;
    rules[TD_VALS_DASH][id] = 25;
    rules[TD_VALS_DASH][semicol] = 25;
    rules[TD_VALS_DASH][cb_cl] = 25;
    rules[TD_VALS_DASH][declare] = 25;
    rules[INT_LIST][num] = 26;
    rules[INT_LIST_DASH][cb_cl] = 27;
    rules[INT_LIST_DASH][semicol] = 28;
    rules[THD_VALS][r1] = 29;
    rules[THD_VALS_DASH][r1] = 30;
    rules[THD_VALS_DASH][id] = 31;
    rules[THD_VALS_DASH][semicol] = 31;
    rules[THD_VALS_DASH][cb_cl] = 31;
    rules[THD_VALS_DASH][declare] = 31;
    rules[INT_LIST_LIST][num] = 32;
    rules[INT_LIST_LIST_DASH][cb_cl] = 34;
    rules[INT_LIST_LIST_DASH][semicol] = 33;
    rules[INT_VAR_LIST][num] = 35;
    rules[INT_VAR_LIST_DASH][cb_cl] = 36;
    rules[INT_VAR_LIST_DASH][semicol] = 36;
    rules[INT_VAR_LIST_DASH][num] = 37;
    rules[ARRAY_RANGE][sq_op] = 38;
    rules[ARRAY_RANGE_DASH][sq_op] = 39;
    rules[ARRAY_RANGE_DASH][of] = 40;
    rules[IDX][id] = 41;
    rules[IDX][num] = 42;
    rules[VAR_LIST][id] = 43;
    rules[MULT_ID][id] = 44;
    rules[MULT_ID_DASH][col] = 45;
    rules[MULT_ID_DASH][id] = 46;
    rules[ASSIGN_STATEMENT][id] = 47;
    rules[EXPR][id] = 48;
    rules[EXPR][num] = 48;
    rules[EXPR_DASH][or_op] = 49;
    rules[EXPR_DASH][add_op] = 49;
    rules[EXPR_DASH][sub_op] = 49;
    rules[EXPR_DASH][id] = 50;
    rules[EXPR_DASH][cb_cl] = 50;
    rules[EXPR_DASH][declare] = 50;
    rules[ASO_OP][or_op] = 53;
    rules[ASO_OP][sub_op] = 52;
    rules[ASO_OP][add_op] = 51;
    rules[TERM][id] = 54;
    rules[TERM][num] = 54;
    rules[TERM_DASH][and_op] = 55;
    rules[TERM_DASH][mul_op] = 55;
    rules[TERM_DASH][div_op] = 55;
    rules[TERM_DASH][or_op] = 56;
    rules[TERM_DASH][sub_op] = 56;
    rules[TERM_DASH][add_op] = 56;
    rules[TERM_DASH][id] = 56;
    rules[TERM_DASH][cb_cl] = 56;
    rules[TERM_DASH][declare] = 56;
    rules[MDA_OP][and_op] = 59;
    rules[MDA_OP][mul_op] = 58;
    rules[MDA_OP][div_op] = 57;
    rules[EPS][or_op] = 60;
    rules[EPS][sub_op] = 60;
    rules[EPS][add_op] = 60;
    rules[EPS][cb_cl] = 60;
    rules[EPS][id] = 60;
    rules[EPS][semicol] = 60;
    rules[EPS][declare] = 60;
    rules[EPS][col] = 60;   

    // for(int i = 0;i<39;i++)
    // {
    //     for(int j = 0;j<31;j++)
    //         printf("%d\t",rules[i][j]);
    //     printf("\n");
    // }
}
int main() {

        grammar G;
        G.grammar_rules = (cell_node*)malloc(NO_OF_RULES*sizeof(cell_node));
        char grammarname[50] = "grammar.txt";
        readGrammar(grammarname,G.grammar_rules);
        char programname[50] = "program.txt";
        tokenStream* head = (tokenStream*)malloc(sizeof(tokenStream)); 
        tokeniseSourcecode(programname,head);
        // generateParseTree(head,G);
        return 0;
}
void generateParseTree(tokenStream* s,grammar G)
{
    int ** rules = hardCodedRules();
    initialiseRules(rules);
}