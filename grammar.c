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

        // printf("%s -> ", token);

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

            // printf("%s ",token);

            rhs->next = malloc(sizeof(rhs_node));
            rhs = rhs->next;
            token = strtok(NULL," \t\r\n");            
        }
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

    p[1][7] = 1;
    p[2][7] = 4;
    p[3][7] = 5;
    p[4][7] = 7;
    p[16][7] = 25;
    p[20][7] = 31;
    p[27][7] = 41;
    p[28][7] = 43;
    p[29][7] = 44;
    p[30][7] = 46;
    p[31][7] = 47;
    p[32][7] = 48;
    p[33][7] = 50;
    p[35][7] = 54;
    p[36][7] = 56;
    p[38][7] = 60;
    p[17][8] = 26;
    p[21][8] = 32;
    p[23][8] = 35;
    p[24][8] = 37;
    p[27][8] = 42;
    p[32][8] = 48;
    p[35][8] = 54;
    p[11][11] = 18;
    p[12][11] = 20;
    p[14][11] = 22;
    p[25][11] = 38;
    p[26][11] = 39;
    p[16][12] = 25;
    p[18][12] = 28;
    p[20][12] = 31;
    p[22][12] = 33;
    p[24][12] = 36;
    p[38][12] = 60;
    p[1][13] = 2;
    p[16][13] = 25;
    p[18][13] = 27;
    p[20][13] = 31;
    p[22][13] = 34;
    p[24][13] = 36;
    p[33][13] = 50;
    p[36][13] = 56;
    p[38][13] = 60;
    p[30][16] = 45;
    p[38][16] = 60;


    return p;
}
void initialiseRules(int **rules)
{
    rules[PROGRAM][program] = 0;
    for(int i = 0;i<39;i++)
    {
        for(int j = 0;j<31;j++)
            printf("%d ",PROGRAM);
        printf("\n");
    }
}
int main() {
        grammar = (cell_node*)malloc(NO_OF_RULES*sizeof(cell_node));
        char grammarname[50] = "grammar.txt";
        readGrammar(grammarname,grammar);
        char programname[50] = "program.txt";
        tokenStream* head = (tokenStream*)malloc(sizeof(tokenStream)); 
        tokeniseSourcecode(programname,head);
        int ** rules = hardCodedRules();
        initialiseRules(rules);
        return 0;
}