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

        curr[linenum].lhs 
        curr[linenum].first_rhs = malloc(sizeof(rhs_node));
        
        curr[linenum].first_rhs->nt
        linenum++;

    }

}
