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
    if(!strcmp(lex,"program"))
        return program;
    else if(!strcmp(lex,"()"))
        return rbs;
    else if(!strcmp(lex,"{"))
        return cb_op;
    else if(!strcmp(lex,"}"))
        return cb_cl;
    else if(!strcmp(lex,"declare"))
        return declare;
    else if(!strcmp(lex,":"))
        return col;
    else if(!strcmp(lex,"list"))
        return list;
    else if(!strcmp(lex,"of"))
        return of;
    else if(!strcmp(lex,"variables"))
        return variables;
    else if(!strcmp(lex,";"))
        return semicol;
    else if(!strcmp(lex,"integer"))
        return integer;
    else if(!strcmp(lex,"boolean"))
        return boolean;
    else if(!strcmp(lex,"real"))
        return real;
    else if(!strcmp(lex,"array"))
        return array;
    else if(!strcmp(lex,"jagged"))
        return jagged;
    else if(!strcmp(lex,".."))
        return ddot;
    else if(!strcmp(lex,"R1"))
        return R1;
    else if(!strcmp(lex,"["))
        return sq_op;
    else if(!strcmp(lex,"]"))
        return sq_cl;
    else if(!strcmp(lex,"size"))
        return size;
    else if(!strcmp(lex,"values"))
        return values;
    else if(!strcmp(lex,"+"))
        return add_op;
    else if(!strcmp(lex,"-"))
        return sub_op;
    else if(!strcmp(lex,"*"))
        return mul_op;
    else if(!strcmp(lex,"/"))
        return div_op;

    for(int i=0;i<strlen(lex);i++) {

        if(lex[i]-'0' < 0 || lex[i]-'0' > 9) {
            return id;
        }
    }
    return num;

}