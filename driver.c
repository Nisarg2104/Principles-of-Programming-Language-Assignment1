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
    int fc[2] = {0,0};
    parseTree *t = calloc(1,sizeof(parseTree));
    typeExpressionTable T=calloc(1,sizeof(type_expression_table));
    do{
        
        printf("Please enter a choice(0-5): \n");
        scanf("%d",&choice);
       
        switch(choice){
            case 0: break;
            case 1:
                if(!fc[0]){
                    createParseTree(t,head,G);
                    fc[0]=1;
                }
                else{
                    printf("Parse Tree already created!\n");
                }
                break;
            case 2:
                if(!fc[0]){
                    printf("Warning! Parse Tree not created! Creating...\n");
                    createParseTree(t,head,G);
                    fc[0]=1;
                }
                if(!fc[1]){
                    traverseParseTree(t,T);
                    fc[1]=1;
                }
                else
                {
                    printf("Type Expression Table already created!\n");
                }
                
                
                break;
            case 3:
                if(!fc[0]){
                    printf("Warning! Parse Tree not created! Creating...\n");
                    createParseTree(t,head,G);
                    fc[0]=1;
                    
                }
                if(!fc[1]){
                    printf("Warning! Type Expression Table not created! Creating...\n");
                    traverseParseTree(t,T);
                    fc[1]=1;    
                }
                printParseTree(t);
                break;
            case 4:
                if(!fc[0]){
                    printf("Warning! Parse Tree not created! Creating...\n");
                    createParseTree(t,head,G);
                    fc[0]=1;
                    
                }
                if(!fc[1]){
                    printf("Warning! Type Expression Table not created! Creating...\n");
                    traverseParseTree(t,T);
                    fc[1]=1;
                    
                }
                printTypeExpressionTable(T);
                break;
            case 5:
                printf("Enter program file: \n");
                scanf("%s",programname);
                head = (tokenStream*)calloc(1,sizeof(tokenStream)); 
                tokeniseSourcecode(programname,head);
                int choice;
                fc[0] = 0;
                fc[1] = 0;
                t = calloc(1,sizeof(parseTree));
                T=calloc(1,sizeof(type_expression_table));
                break;
            default:
                printf("Invalid choice!\n");
        }
    }while(choice!=0);
}

// boolean = saare boolean operators saare goodvariable saare boolean variables
// integer/real = saare not a boolean a operator saare goodvariable(can be integer can be real)
// integer -> a [ x y ]
// ri.range[0] < num[i] < r1.range[1]
// count == range size
