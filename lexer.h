#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include<stdio.h>
#define MAX_VAR_NAME_LEN 21

typedef enum {
    #include "terminals.txt"
} terminal;

typedef enum{
    #include "non_terminals.txt"
} non_terminal;


struct TOKEN
{
    char lexeme[MAX_VAR_NAME_LEN];
    int line_num;
    terminal token_name;
    struct TOKEN* nextToken;
};
typedef struct TOKEN tokenStream;

void tokeniseSourcecode( char* filename, tokenStream *s);
terminal returnTokenstreamTerminalVal(char* lex);
non_terminal returnNonTerminalVal(char* lex);
terminal returnTerminalVal(char* lex);