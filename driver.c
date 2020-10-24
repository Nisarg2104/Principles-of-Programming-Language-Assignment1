#include "typeExprTable.h"

int main(){
    
    char programname[201];

    grammar G;
    G.grammar_rules = (cell_node*)calloc(NO_OF_RULES,sizeof(cell_node));
    for(int  i = 0;i<NO_OF_RULES;i++)
    {
        (G.grammar_rules)[i].first_rhs = NULL;
        (G.grammar_rules)[i].last_rhs = NULL;
    }
    char grammarname[50] = "grammar.txt";
    readGrammar(grammarname,G.grammar_rules);


    printf("Enter program file: \n");
    scanf("%s",programname);
    tokenStream* head = (tokenStream*)calloc(1,sizeof(tokenStream)); 
    tokeniseSourcecode(programname,head);
    int choice;
    
    do{
        
        printf("Please enter a choice(0-4): \n");
        scanf("%d",&choice);
        parseTree *t = calloc(1,sizeof(parseTree));
        typeExpressionTable T;
        T.dataTypes=NULL;
        switch(choice){
            case 0: break;
            case 1:
                t = calloc(1,sizeof(parseTree));
                createParseTree(t,head,G);
                break;
            case 2:
                t = calloc(1,sizeof(parseTree));
                createParseTree(t,head,G);
                traverseParseTree(t,  T);
                break;
            case 3:
                t = calloc(1,sizeof(parseTree));
                createParseTree(t,head,G);
                traverseParseTree(t,  T);
                printParseTree(t);
                break;
            case 4: 
                t = calloc(1,sizeof(parseTree));
                createParseTree(t,head,G);
                traverseParseTree(t,  T);
                printParseTree(t);
                //printTypeExpressionTable(T);
                break;
            default:
                printf("Invalid choice!\n");
        }
    }while(choice!=0);
}