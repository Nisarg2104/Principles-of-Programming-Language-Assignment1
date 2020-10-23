#include "nisarg.h"
#include<assert.h>

stack* create_stack(){
    stack *s = (stack *) calloc(1,sizeof(stack));
    s->capacity=1;
    s->top=-1;
    s->arr=(rhs_node *)calloc(1,sizeof(rhs_node));
    return s;
}

void push(stack* s,rhs_node r){
    if(s->capacity==s->top+1){
        s->capacity=s->capacity*2;
        s->arr=(rhs_node*) realloc(s->arr,sizeof(rhs_node)*s->capacity); 
    }
    s->top++;
    s->arr[s->top]=r;
}

rhs_node pop(stack *s){
    if(s->top==-1) printf("Stack empty");
    else {
        s->top=s->top-1;
    }
    return s->arr[s->top+1];
}
void runTerm(int x)
{
    switch(x)
    {
        case 0:
            printf("&&& ");
            break;
        case 1:
            printf("/ ");
            break;
        case 2:
            printf("* ");
            break;
        case 3:
            printf("||| ");
            break;
        case 4:
            printf("- ");
            break;
        case 5:
            printf("+ ");
            break;
        case 6:
            printf("= ");
            break;
        case 7:
            printf("ID ");
            break;
        case 8:
            printf("NUM ");
            break;
        case 9:
            printf("] ");
            break;
        case 10:
            printf(".. ");
            break;
        case 11:
            printf("[ ");
            break;
        case 12:
            printf("; ");
            break;
        case 13:
            printf("} ");
            break;
        case 14:
            printf("{ ");
            break;
        case 15:
            printf("values ");
            break;
        case 16:
            printf(": ");
            break;
        case 17:
            printf("size ");
            break;
        case 18:
            printf("R1 ");
            break;
        case 19:
            printf("integer ");
            break;
        case 20:
            printf("of ");
            break;
        case 21:
            printf("array ");
            break;
        case 22:
            printf("jagged ");
            break;
        case 23:
            printf("real ");
            break;
        case 24:
            printf("boolean ");
            break;
        case 25:
            printf("variables ");
            break;
        case 26:
            printf("list ");
            break;
        case 27:
            printf("declare ");
            break;
        case 28:
            printf("rbs ");
            break;
        case 29:
            printf("program ");
            break;
        case 30:
            printf("eps ");
            break;
        default:
            printf("Token Don't Exist");
    }
}

char** initNonTerms() {
    FILE* non_terms = fopen("non_terminals.txt","r");
    char**tokens = calloc(44,sizeof(char*));
    int index = 0;
    char BUF [MAX_VAR_NAME_LEN];
    while(!feof(non_terms)) {
        fgets(BUF,MAX_VAR_NAME_LEN+1,non_terms);
        tokens[index] = (char*)calloc(MAX_VAR_NAME_LEN,1);
        strcpy(tokens[index],strtok(BUF,","));
        index++;
    }
    fclose(non_terms);
    return tokens;
}
// link bhejo koihttps://meet.google.com/cxy-zpti-xxp?authuser=1

// void createParseTree( parseTree *t, tokenStream *s, grammar G){
//     stack *main_stack;
//     int ** rules ;
//     rules=hardCodedRules();
//     rules= initialiseRules(rules);
    
//     s = s->nextToken;
//     // printf("%s",s->lexeme); 
//     main_stack=create_stack();
//     tokenStream *curr_tok = s;
//     rhs_node prog_node;

//     prog_node.is_terminal=false;
//     prog_node.next=NULL;
//     prog_node.prev=NULL;
//     prog_node.non_term=PROGRAM;

//     push(main_stack,prog_node);
    
//     t->left_most_child=NULL;
//     t->non_term=PROGRAM;
//     t->is_terminal=false;
//     t->right_most_child=NULL;
//     t->parent=NULL;

//     parseTree *curr_parse_tree_parent = t;
//     int count = 0;
//     do{

//         rhs_node temp;
//         temp=pop(main_stack);

//         int curr_rule =  rules[temp.non_term][curr_tok->token_name]; 
        
//         cell_node curr_grammar = G.grammar_rules[curr_rule];
//         printf("temp.non_term= %d curr_tok->token_name=%d curr_rule=%d\n",temp.non_term,curr_tok->token_name,curr_rule);
//         if(curr_grammar.lhs != temp.non_term)
//         {
//             printf("%d\n",count);
//             printf("%s %d %d\n",curr_tok->lexeme,temp.non_term, curr_tok->line_num);
//         }
//         assert(curr_grammar.lhs == temp.non_term);

//         rhs_node *curr_rhs=curr_grammar.last_rhs;

//         while(curr_rhs!=NULL){
//             //if(main_stack->arr[main_stack->top].is_terminal)
//             //   printf("%d %d\n",main_stack->arr[main_stack->top].is_terminal,main_stack->arr[main_stack->top].term);
//             //else
//             //{
//             //    printf("%d %d\n",main_stack->arr[main_stack->top].is_terminal,main_stack->arr[main_stack->top].non_term);
//             //}
// 			    push(main_stack,*curr_rhs);
        
//             if(curr_rhs->is_terminal)
//                 printf("%d %d\n",curr_rhs->is_terminal,curr_rhs->term);
//             else
//             {
//                 printf("%d %d\n",curr_rhs->is_terminal,curr_rhs->non_term);
//             }
//             curr_rhs=curr_rhs->prev;
//         }
//         int flag = 0;
//         while(main_stack->arr[main_stack->top].is_terminal){
//             if(main_stack->arr[main_stack->top].term==curr_tok->token_name){
//                 runVal(main_stack->arr[main_stack->top].term);
//                 printf(" %s\n",curr_tok->lexeme);
//                 *curr_rhs = pop(main_stack);
                
//                 curr_tok=curr_tok->nextToken;
//             }
//             else if(main_stack->arr[main_stack->top].term == 30)
//             {
//                 pop(main_stack);
//                 // printf("Chala\n");
//             }
//             else{
//                 // printf("%d %d\n",main_stack->arr[main_stack->top].term,curr_tok->token_name);
//                 // printf("fkin error!");
//                 flag = 1;
//                 break;
//             }
            
//         }
//         count++;
//         if(flag)
//             break;

//     }while(main_stack->top!=-1);

//     if(curr_tok==NULL){
//         printf("Yayy!");
//     }


// }