#include "grammar.h"

void readGrammar(char* filename,cell_node *grammar) {
    int linenum = 0;
    FILE* grFile = fopen(filename, "r");
    cell_node* head = grammar;
    cell_node* curr = head;
    while(!feof(grFile)) {
        char* BUF = malloc(200);
        fgets(BUF,200,grFile);
        char *token;
        token = strtok(BUF, " \t\r\n");

        curr[linenum].lhs = returnNonTerminalVal(token);
        curr[linenum].first_rhs = malloc(sizeof(rhs_node));
        rhs_node* rhs = curr[linenum].first_rhs;

        token = strtok(NULL," \t\r\n");
        
        while(token != NULL) {
            token = strtok(NULL," \t\r\n");
            if(isupper(token[0])) {
                rhs->is_terminal = false;
                rhs->non_term = returnNonTerminalVal(token);
            }
            else
            {
                rhs->is_terminal = true;
                rhs->term = returnTerminalVal(token);
            }

            rhs->next = malloc(sizeof(rhs_node));
            rhs = rhs->next;
            
        }
        linenum++;

    }

}
