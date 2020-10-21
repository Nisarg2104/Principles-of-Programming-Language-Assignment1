#include "lexer.h"

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


non_terminal returnNonTerminalVal(char* lex) {

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
    else if(!strcmp(lex,"JAGGED_INIT_DASH"))
        return JAGGED_INIT_DASH;
    else if(!strcmp(lex,"JAGGED_INIT"))
        return JAGGED_INIT;
    else if(!strcmp(lex,"TD_INIT"))
        return TD_INIT;
    else if(!strcmp(lex,"THD_INIT"))
        return THD_INIT;
    else if(!strcmp(lex,"TD_VALS_DASH"))
        return TD_VALS_DASH;
    else if(!strcmp(lex,"THD_VALS_DASH"))
        return THD_VALS_DASH;
    else if(!strcmp(lex,"TD_VALS"))
        return TD_VALS;
    else if(!strcmp(lex,"THD_VALS"))
        return THD_VALS;
    else if(!strcmp(lex,"INT_LIST"))
        return INT_LIST;
    else if(!strcmp(lex,"INT_LIST_LIST"))
        return INT_LIST_LIST;
    else if(!strcmp(lex,"INT_LIST_LIST_DASH"))
        return INT_LIST_LIST_DASH;    
    else if(!strcmp(lex,"INT_VAR_LIST"))
        return INT_VAR_LIST;
    else if(!strcmp(lex,"INT_VAR_LIST_DASH"))
        return INT_VAR_LIST_DASH;
    else if(!strcmp(lex,"ARRAY_RANGE"))
        return ARRAY_RANGE;
    else if(!strcmp(lex,"ARRAY_RANGE_DASH"))
        return ARRAY_RANGE_DASH;
    else if(!strcmp(lex,"IDX"))
        return IDX;
    else if(!strcmp(lex,"MULT_ID"))
        return MULT_ID;
    else if(!strcmp(lex,"MULT_ID_DASH"))
        return MULT_ID_DASH;
    else if(!strcmp(lex,"EXPR"))
        return EXPR;
    else if(!strcmp(lex,"EXPR_DASH"))
        return EXPR_DASH;
    else if(!strcmp(lex,"TERM"))
        return TERM;
    else if(!strcmp(lex,"TERM_DASH"))
        return TERM_DASH;
    else if(!strcmp(lex,"AS_OP"))
        return AS_OP;
    else if(!strcmp(lex,"MD_OP"))
        return MD_OP;
    else if(!strcmp(lex,"EPS"))
        return EPS;


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
        return r1;
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
    else if(!strcmp(lex,"="))
        return eq_op;
    else if(!strcmp(lex,""))
        return eps;
    for(int i=0;i<strlen(lex);i++) {

        if(lex[i]-'0' >= 0 && lex[i]-'0' > 9) {
            return id;
        }
    }
    return num;

}