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
        grammar[linenum].first_rhs = malloc(sizeof(rhs_node));
        rhs_node* rhs = grammar[linenum].first_rhs;

        printf("%s -> ", token);

        token = strtok(NULL," \t\r\n");
        token = strtok(NULL," \t\r\n");

        
        while(token != NULL) {
            if(isupper(token[0])) {
                rhs->is_terminal = false;
                rhs->non_term = returnNonTerminalVal(token);
            }
            else
            {
                rhs->is_terminal = true;
                rhs->term = returnTerminalVal(token);
            }

            printf("%s ",token);

            rhs->next = malloc(sizeof(rhs_node));
            rhs = rhs->next;
            token = strtok(NULL," \t\r\n");            
        }
        printf("\n");
        linenum++;

    }

}

int main() {
        grammar = (cell_node*)malloc(NO_OF_RULES*sizeof(cell_node));

        char filename[50] = "grammar.txt";
        readGrammar(filename,grammar);
        return 0;
}