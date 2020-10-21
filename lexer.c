#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tokeniseSourcecode( char* filename, tokenStream *s) {
    FILE* code = fopen(filename,"r");
    tokenStream *head = s;
    tokenStream* curr = head;
    char* BUF = (char*)malloc(200);
    int linenum = 0;
    while(!feof(code)){
        linenum++;
        fgets(BUF,200,code);
        char *token;
        token = strtok(BUF, " \t\r\n");
   
        while( token != NULL) 
        {
            curr->nextToken = malloc(sizeof(tokenStream));
            curr = curr->nextToken;
            curr->line_num = linenum;
            strcpy(curr->lexeme,token);
            // curr->token_name = 
            // printf( " %s\n", token);
            token = strtok(NULL, " \t\r\n");
        }
    }
    curr = head->nextToken;
    while(curr != NULL)
    {
        printf(" %s %d\n",curr->lexeme,curr->line_num);
        curr = curr->nextToken;
    }
    
}

int main() {
    char filename[50] = "program.txt";
    tokenStream* head = (tokenStream*)malloc(sizeof(tokenStream)); 
    tokeniseSourcecode(filename,head);
    return 0;
}

terminal returnTerminalVal(char* lex) {

    if(!strcmp(lex,"PROGRAM"))
        return PROGRAM;
    else if(!strcmp(lex,"STATEMENTS"))
        return STATEMENTS;
    else if(!strcmp(lex,"STATEMENT"))
        return STATEMENT;
    else if(!strcmp(lex,"DECLARE_STATEMENT"))
        return DECLARE_STATEMENT;
    else if(!strcmp(lex,"ASSIGN_STATEMENT"))
        return ASSIGN_STATEMENT;
    else if(!strcmp(lex,"SINGLE_DECLARE"))
        return SINGLE_DECLARE;
    else if(!strcmp(lex,"MULTI_DECLARE"))
        return MULTI_DECLARE;
    else if(!strcmp(lex,"TYPE"))
        return TYPE;
    else if(!strcmp(lex,"VAR_LIST"))
        return VAR_LIST;
    
     else if(!strcmp(lex,"PRIM_TYPE"))
        return PRIM_TYPE;
    else if(!strcmp(lex,"ARRAY"))
        return ARRAY;
    else if(!strcmp(lex,"RECTANGULAR"))
        return RECTANGULAR;
    else if(!strcmp(lex,"ASSIGN_STATEMENT"))
        return ASSIGN_STATEMENT;
    else if(!strcmp(lex,"JAGGED"))
        return JAGGED;
    else if(!strcmp(lex,"JAGGED_INIT"))
        return JAGGED_INIT;
    else if(!strcmp(lex,"2D_INIT"))
        return 2D_INIT;
    else if(!strcmp(lex,"3D_INIT"))
        return 3D_INIT;


    else if(!strcmp(lex,"2D_VALS"))
        return 2D_VALS;
    else if(!strcmp(lex,"3D_VALS"))
        return 3D_VALS;
    else if(!strcmp(lex,"INT_LIST"))
        return INT_LIST;
    else if(!strcmp(lex,"INT_LIST_LIST"))
        return INT_LIST_LIST;
    else if(!strcmp(lex,"INT_VAR_LIST"))
        return INT_VAR_LIST;
    else if(!strcmp(lex,"ARRAY_RANGE"))
        return ARRAY_RANGE;
    else if(!strcmp(lex,"IDX"))
        return IDX;
    else if(!strcmp(lex,"MULT_ID"))
        return MULT_ID;

     else if(!strcmp(lex,"EQ_UP"))
        return EQ_UP;
    else if(!strcmp(lex,"EXPR"))
        return EXPR;
    else if(!strcmp(lex,"TERM"))
        return TERM;
    else if(!strcmp(lex,"AS_OP"))
        return AS_OP;
    else if(!strcmp(lex,"MD_OP"))
        return MD_OP;

    
}
terminal returnTerminalVal(char* lex) {
    if(!strcmp(lex,"program"))
        return program;
    else if(!strcmp(lex,"rbs"))
        return rbs;
    else if(!strcmp(lex,"cb_op"))
        return cb_op;
    else if(!strcmp(lex,"cb_cl"))
        return cb_cl;
    else if(!strcmp(lex,"declare"))
        return declare;
    else if(!strcmp(lex,"id"))
        return id;
    else if(!strcmp(lex,"col"))
        return col;
    else if(!strcmp(lex,"list"))
        return list;
    else if(!strcmp(lex,"of"))
        return
}