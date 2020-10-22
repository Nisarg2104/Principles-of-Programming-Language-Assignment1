#include "nisarg.h"
#include<assert.h>

stack* create_stack(){
    stack *s = (stack *) malloc(sizeof(stack));
    s->capacity=1;
    s->top=-1;
    s->arr=(rhs_node *)malloc(sizeof(rhs_node));
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


void createParseTree( parseTree *t, tokenStream *s, grammar G){
    stack *main_stack;
     int ** rules ;
     rules=hardCodedRules();
    rules= initialiseRules(rules);

    
    s = s->nextToken;
    printf("%s",s->lexeme); 
    main_stack=create_stack();
    tokenStream *curr_tok = s;
    rhs_node prog_node;

    prog_node.is_terminal=false;
    prog_node.next=NULL;
    prog_node.prev=NULL;
    prog_node.non_term=PROGRAM;

    push(main_stack,prog_node);
    
    
   
    
    t=(parseTree*) malloc(sizeof(parseTree));
    t->left_most_child=NULL;
    t->nt=PROGRAM;
    t->right_most_child=NULL;
    t->parent=NULL;

    parseTree *curr;
    curr=t;
    // // change do while to for 1 iteration only
    int count = 0;
    do{

        rhs_node temp;
        temp=pop(main_stack);

        int curr_rule =  rules[temp.non_term][curr_tok->token_name]; 

        cell_node curr_grammar = G.grammar_rules[curr_rule];
        printf("%d\n",count);
        // printf("%d %d\n",curr_rule,temp.non_term);
        assert(curr_grammar.lhs == temp.non_term);

        rhs_node *curr_rhs=curr_grammar.last_rhs;

        while(curr_rhs!=NULL){
            push(main_stack,*curr_rhs);
            if(curr_rhs->is_terminal)
                printf("%d %d\n",curr_rhs->is_terminal,curr_rhs->term);
            else
            {
                printf("%d %d\n",curr_rhs->is_terminal,curr_rhs->non_term);
            }
            curr_rhs=curr_rhs->prev;
        }
        int flag = 0;
        while(main_stack->arr[main_stack->top].is_terminal){
            if(main_stack->arr[main_stack->top].term==s->token_name){
                pop(main_stack);
                printf("Chala");
                curr_tok=curr_tok->nextToken;
            }
            else{
                printf("%d %d\n",main_stack->arr[main_stack->top].term,s->token_name);
                printf("fkin error!");
                flag = 1;
                break;
            }
            
        }
        count++;
        if(flag)
            break;

    }while(main_stack->top!=-1);

    if(curr_tok==NULL){
        printf("Yayy!");
    }


}