#include "nisarg.h"


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
            parseTree* nodeToAdd = malloc(sizeof(parseTree));
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
                runVal(traverseNode->term);
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

}