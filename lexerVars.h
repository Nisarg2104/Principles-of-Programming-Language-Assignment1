#include<stdio.h>
#define MAX_VAR_NAME_LEN 50
struct TOKEN
{
    union{
        char lexeme[MAX_VAR_NAME_LEN];
        int int_val;
        double real_val;
    };
    
    int line_num;
    token token_name;
};


typedef enum {
    #include "tokens.txt"
} token;

typedef enum{
    #include "non_terminals.txt"
} non_terminal;

