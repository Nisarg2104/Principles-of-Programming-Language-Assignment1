#include "typeExprTable.h"

stack* create_stack(){
    stack *s = (stack *) calloc(1,sizeof(stack));
    s->capacity=1;
    s->top=-1;
    s->arr=(parseTree *)calloc(1,sizeof(parseTree));
    return s;
}

void push(stack* s,parseTree r){
    if(s->capacity==s->top+1){
        s->capacity=s->capacity*2;
        s->arr=(parseTree*) realloc(s->arr,sizeof(parseTree)*s->capacity); 
    }
    s->top++;
    s->arr[s->top]=r;
}

parseTree pop(stack *s){
    if(s->top==-1) printf("Stack empty");
    else {
        s->top=s->top-1;
    }
    return s->arr[s->top+1];
}

char* returnPrimitiveType(int x) {
    char *type  = malloc(15);
    switch (x)
    {
    case integer:
        strcpy(type,"<type=integer>");
        break;
    case boolean:
        strcpy(type,"<type=boolean>");
        break;
    case real:
        strcpy(type,"<type=real>");
        break;
    }
    return type;
}

void traverseParseTree(parseTree *t, typeExpressionTable T) {
    parseTree* traverseNode = t;
    int flag = 0;
    stack * mainStack = create_stack();
    typeExpression* currTypeExpression;
do
{
    traverseNode->currNode = traverseNode->left_most_child;
        if(!traverseNode->is_terminal && traverseNode->non_term == DECLARE_STATEMENT) {
            currTypeExpression = (typeExpression*) calloc(1,sizeof(typeExpression));
        }
        if(traverseNode->parent !=NULL) {
            if(traverseNode->is_terminal && traverseNode->parent->non_term == PRIM_TYPE) {
            currTypeExpression->dataType = _prim;
            currTypeExpression->typeName=returnPrimitiveType(traverseNode->term);
            currTypeExpression->arrayType = NA;
        }
        if(!traverseNode->is_terminal && traverseNode->parent->non_term == ARRAY) {
            if(traverseNode->non_term == JAGGED) {
                currTypeExpression->dataType = _jagged;
                currTypeExpression->arrayType = NA;
                currTypeExpression->dimensions = 2;
            }
            else
            {
                currTypeExpression->dataType = _array;
                currTypeExpression->dimensions = 1;
            }
            
        }
    }
    

    if(traverseNode->is_terminal && traverseNode->term == id && (traverseNode->parent->non_term == MULT_ID || traverseNode->parent->non_term == VAR_LIST || traverseNode->parent->non_term == SINGLE_DECLARE)) {
        push(mainStack,*traverseNode);
    }

    if(traverseNode->is_terminal && traverseNode->term == id && traverseNode->parent->non_term == IDX && traverseNode->parent->right_sibling->is_terminal && (traverseNode->parent->right_sibling->term == ddot || traverseNode->parent->right_sibling->term == sq_cl)) {
        if(currTypeExpression->dimensions == 1) {
            if(traverseNode->parent->right_sibling->term == ddot) {
                currTypeExpression->range_R1[0] = traverseNode->lexeme;
            }
            else {
                currTypeExpression->range_R1[1] = traverseNode->lexeme;
            }
        }
        else {
            if(currTypeExpression->dimensions == 2) {
                currTypeExpression->rectArrayRanges = calloc(1,sizeof(rect_array_ranges));
                currTypeExpression->rectArrayRanges->first = calloc(1,sizeof(rect_array));
                currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->first;

                if(traverseNode->parent->right_sibling->term == ddot) {
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                }
            }
            else {
                currTypeExpression->rectArrayRanges->last->next = calloc(1,sizeof(rect_array));
                currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->last->next;

                if(traverseNode->parent->right_sibling->term == ddot) {
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                }
            }
            
        }
        currTypeExpression->dimensions++;
    }
    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == IDX && traverseNode->parent->right_sibling->is_terminal && (traverseNode->parent->right_sibling->term == ddot || traverseNode->parent->right_sibling->term == sq_cl)) {
        if(currTypeExpression->dimensions == 1) {
            if(traverseNode->parent->right_sibling->term == ddot) {
                currTypeExpression->range_R1[0] = traverseNode->lexeme;
                currTypeExpression->tdjaggedArrayRange.low = atoi(traverseNode->lexeme);
            }
            else {
                currTypeExpression->range_R1[1] = traverseNode->lexeme;
                if(currTypeExpression->dataType == _jagged) {
                    currTypeExpression->tdjaggedArrayRange.high = atoi(traverseNode->lexeme);
                    currTypeExpression->tdjaggedArrayRange.ranges = calloc(currTypeExpression->tdjaggedArrayRange.high-currTypeExpression->tdjaggedArrayRange.low+1,sizeof(char*));
                    currTypeExpression->tdjaggedArrayRange.index = 0;
                }
            }
        }
        else {
            if(currTypeExpression->dimensions == 2) {
                currTypeExpression->rectArrayRanges = calloc(1,sizeof(rect_array_ranges));
                currTypeExpression->rectArrayRanges->first = calloc(1,sizeof(rect_array));
                currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->first;

                if(traverseNode->parent->right_sibling->term == ddot) {
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                }
            }
            else {
                currTypeExpression->rectArrayRanges->last->next = calloc(1,sizeof(rect_array));
                currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->last->next;

                if(traverseNode->parent->right_sibling->term == ddot) {
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                }
            }
            
        }
        currTypeExpression->dimensions++;
    }

    if(traverseNode->is_terminal && traverseNode->term == sq_op && traverseNode->parent->non_term == THD_INIT) {
        currTypeExpression->dimensions = 3;
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == INT_LIST) {
        currTypeExpression->tdjaggedArrayRange.ranges[currTypeExpression->tdjaggedArrayRange.index] = traverseNode->lexeme;
        currTypeExpression->tdjaggedArrayRange.index++;
    }

    //propogation code starts here

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
            if(!traverseNode->is_terminal && traverseNode->non_term == TYPE) {
                while(mainStack->top!=-1) {
                    parseTree currID = pop(mainStack);
                    printf("%s ", currID.lexeme);
                    currID.type = currTypeExpression;
                    if(currTypeExpression->dataType == _array) {
                        printf("[ %s .. %s ] ",currTypeExpression->range_R1[0],currTypeExpression->range_R1[1]);
                        rect_array* currRectArr = currTypeExpression->rectArrayRanges->first;
                        do
                        {
                            printf("[ %s .. %s ] ",currRectArr->range[0],currRectArr->range[1]);
                        } while (currRectArr != currTypeExpression->rectArrayRanges->last);
                        
                    }
                    printf("\n");
                    if(T.dataTypes==NULL) {
                        T.dataTypes =(dataType*) malloc(sizeof(dataType));
                        T.variables = 1;
                    }
                    else {
                        T.variables++;
                        T.dataTypes = (dataType*) realloc(T.dataTypes,T.variables*sizeof(dataType));
                    }
                    T.dataTypes[variables-1].type = currTypeExpression;
                    strcpy(T.dataTypes[variables-1].varName,currID.lexeme);
                    // currID.parent->type = currTypeExpression;
                }
            }

        }
        if(flag)
            break;
        traverseNode->currNode = traverseNode->currNode->right_sibling;
        traverseNode = traverseNode->currNode;
    }
}
while (traverseNode->parent != NULL || traverseNode->right_sibling != NULL);
    
    
}