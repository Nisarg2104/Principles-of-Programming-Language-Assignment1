#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tokeniseSourcecode( char* filename, tokenStream *s) 
{
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
