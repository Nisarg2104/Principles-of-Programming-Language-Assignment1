#include "parser.h"

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
    char**tokens = calloc(43,sizeof(char*));
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


void createParseTree(parseTree *t, tokenStream *s, grammar G) {
    int ** rules ;
    rules=hardCodedRules();
    rules= initialiseRules(rules);

    
    s = s->nextToken;
    // printf("%s",s->lexeme); 

    rhs_node prog_node;

    prog_node.is_terminal=false;
    prog_node.next=NULL;
    prog_node.prev=NULL;
    prog_node.non_term=PROGRAM;
    
    t->left_most_child=NULL;
    t->non_term=PROGRAM;
    t->is_terminal=false;
    t->linenum = 0;
    t->currNode = NULL;
    t->parent=NULL;

    parseTree *traverseNode = t;
    int flag = 0;
    while(s!=NULL) {
        rhs_node temp;
        temp.is_terminal = traverseNode->is_terminal;
        temp.non_term = traverseNode->non_term;
        int ruleNum =  rules[temp.non_term][s->token_name]; 
        
        cell_node curr_grammar = G.grammar_rules[ruleNum];
        assert(curr_grammar.lhs == temp.non_term);

        rhs_node* currRule = curr_grammar.first_rhs;
        while (currRule!=NULL)
        {
            // Make a New Node of Parse Tree
            // Join it with Curr
            parseTree* nodeToAdd = calloc(1,sizeof(parseTree));
            nodeToAdd->parent = traverseNode;
            nodeToAdd->left_most_child = NULL;
            nodeToAdd->right_sibling = NULL;
            nodeToAdd->currNode = NULL;
            nodeToAdd->linenum = -1;
            nodeToAdd->is_terminal = currRule->is_terminal;
            if(currRule->is_terminal)
            {
                nodeToAdd->term = currRule->term;
            }
            else
            {
                nodeToAdd->non_term = currRule->non_term; 
            }
            if(traverseNode->left_most_child == NULL)
            {
                traverseNode->left_most_child = nodeToAdd;
                traverseNode->currNode = nodeToAdd;
            }
            else
            {
                traverseNode->currNode->right_sibling = nodeToAdd;
                traverseNode->currNode = traverseNode->currNode->right_sibling;
            }
            currRule = currRule->next;

        }
        traverseNode->currNode = traverseNode->left_most_child;
        traverseNode = traverseNode->currNode;
        while(traverseNode->parent != NULL && traverseNode->is_terminal)
        {
            // match currPTparent->currNode with tokenStream
            // increment tokenStream
            if(traverseNode->term == s->token_name)
            {
                strcpy(traverseNode->lexeme,s->lexeme);
                traverseNode->linenum = s->line_num;
                runTerm(traverseNode->term);
                printf("\n");
                s = s->nextToken;
                if(s==NULL) break;
            }
            else if(traverseNode->term != eps)
            {
                flag = 1;
                break;
            }
            
            if(traverseNode->right_sibling == NULL)
            {
                while(traverseNode->right_sibling == NULL)
                {
                    traverseNode = traverseNode->parent;
                }
                traverseNode = traverseNode->right_sibling;
            }
            else
            {
                traverseNode = traverseNode->right_sibling;
            }
            // currNode->rightSibling -> null 
            // currPTparent = currPTparent->parent
        }

        if(flag)
        {
            printf("Fuckin Error\n");
            break;
        }     
    }
    assert(s == NULL);
    printf("YAAYYY !!!\n");
    printParseTree(t);
    typeExpressionTable T;
    T.dataTypes = NULL;
    traverseParseTree(t,T);

}

void printParseTree(parseTree *t) {
    parseTree* traverseNode = t;
    printf("%d\n",t->non_term);
    char** printNonTerm = initNonTerms();
    int depth = 0;
    int count = 0;
    int flag = 0;
    do {
        count++;
        traverseNode->currNode = traverseNode->left_most_child;
        traverseNode->is_terminal?runTerm(traverseNode->term):printf("%s ",printNonTerm[traverseNode->non_term]);
        traverseNode->is_terminal?printf("terminal\n"):printf("non terminal\n");
        
        if(traverseNode->currNode != NULL)
            traverseNode = traverseNode->currNode;
        else
        {
            while(traverseNode->is_terminal || traverseNode->currNode->right_sibling == NULL)
            {
                traverseNode = traverseNode->parent;
                if(traverseNode == NULL)
                {
                    flag = 1;
                    break;
                }
            }
            if(flag)
                break;
            traverseNode->currNode = traverseNode->currNode->right_sibling;
            traverseNode = traverseNode->currNode;
        }

    }while(traverseNode->parent != NULL || traverseNode->right_sibling != NULL);

    printf("%d\n",count);
}