#include "grammar.h"
#include <assert.h>
void createParseTree( parseTree *t, tokenStream *s, grammar G);
void printParseTree(parseTree *t);

void runTerm(int x);
char** initNonTerms();