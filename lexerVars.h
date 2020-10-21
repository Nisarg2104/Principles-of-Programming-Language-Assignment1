#include<stdio.h>
#define MAX_VAR_NAME_LEN 50

typedef enum {
    #include "terminals.txt"
} terminal;

typedef enum{
    JAGGED_LIST
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



