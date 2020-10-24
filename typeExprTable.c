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
        currTypeExpression = (typeExpression*) calloc(1,sizeof(typeExpression));    //init new type expr
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
                currTypeExpression->dimensions = 0;
            }
            
        }
    }
    

    if(traverseNode->is_terminal && traverseNode->term == id && (traverseNode->parent->non_term == MULT_ID || traverseNode->parent->non_term == VAR_LIST || traverseNode->parent->non_term == SINGLE_DECLARE)) {
        push(mainStack,*traverseNode);
    }

    if(traverseNode->is_terminal && traverseNode->term == id && traverseNode->parent->non_term == IDX && traverseNode->parent->right_sibling->is_terminal && (traverseNode->parent->right_sibling->term == ddot || traverseNode->parent->right_sibling->term == sq_cl)) {
        if(currTypeExpression->dimensions == 0) {
            if(traverseNode->parent->right_sibling->term == ddot) {
                currTypeExpression->range_R1[0] = traverseNode->lexeme;
            }
            else {
                currTypeExpression->range_R1[1] = traverseNode->lexeme;
                currTypeExpression->dimensions++;
            }
        }
        else {
            if(currTypeExpression->dimensions == 1) {
                
                if(traverseNode->parent->right_sibling->term == ddot) {
                    currTypeExpression->rectArrayRanges = calloc(1,sizeof(rect_array_ranges));
                    currTypeExpression->rectArrayRanges->first = calloc(1,sizeof(rect_array));
                    currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->first;
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                    currTypeExpression->dimensions++;
                }
            }
            else {

                if(traverseNode->parent->right_sibling->term == ddot) {
                    currTypeExpression->rectArrayRanges->last->next = calloc(1,sizeof(rect_array));
                    currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->last->next;
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                    currTypeExpression->dimensions++;
                }
            }
            
        }
    }
    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == IDX && traverseNode->parent->right_sibling->is_terminal && (traverseNode->parent->right_sibling->term == ddot || traverseNode->parent->right_sibling->term == sq_cl)) {
        if(currTypeExpression->dimensions == 0) {
            if(traverseNode->parent->right_sibling->term == ddot) {
                currTypeExpression->range_R1[0] = traverseNode->lexeme;
            }
            else {
                currTypeExpression->range_R1[1] = traverseNode->lexeme;
                currTypeExpression->dimensions++;
            }
        }
        else {
            if(currTypeExpression->dimensions == 1) {
                
                if(traverseNode->parent->right_sibling->term == ddot) {
                    currTypeExpression->rectArrayRanges = calloc(1,sizeof(rect_array_ranges));
                    currTypeExpression->rectArrayRanges->first = calloc(1,sizeof(rect_array));
                    currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->first;
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                    currTypeExpression->dimensions++;
                }
            }
            else {

                if(traverseNode->parent->right_sibling->term == ddot) {
                    currTypeExpression->rectArrayRanges->last->next = calloc(1,sizeof(rect_array));
                    currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->last->next;
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                    currTypeExpression->dimensions++;
                }
            }
            
        }
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == JAGGED) {
        if(traverseNode->right_sibling->term == ddot) {

            currTypeExpression->range_R1[0] = traverseNode->lexeme;
            currTypeExpression->low = atoi(traverseNode->lexeme);
        }
        else {
            currTypeExpression->range_R1[1] = traverseNode->lexeme;
            currTypeExpression->high = atoi(traverseNode->lexeme);
        }
    }

    if(traverseNode->is_terminal && traverseNode->term == of && traverseNode->parent->non_term == TD_INIT) {
        currTypeExpression->tdjaggedArrayRange.ranges = calloc(currTypeExpression->high-currTypeExpression->low+1,sizeof(char*));
        currTypeExpression->tdjaggedArrayRange.index = 0;
    }

    if(traverseNode->is_terminal && traverseNode->term == sq_op && traverseNode->parent->non_term == THD_INIT) {
        currTypeExpression->dimensions = 3;
        currTypeExpression->thdJaggedArrayRange.subRanges = calloc(currTypeExpression->high-currTypeExpression->low+1,sizeof(thd_sub_range));
        currTypeExpression->thdJaggedArrayRange.index = 0;
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == TD_VALS && traverseNode->right_sibling->term == col) {
        currTypeExpression->tdjaggedArrayRange.ranges[currTypeExpression->tdjaggedArrayRange.index] = traverseNode->lexeme;
        currTypeExpression->tdjaggedArrayRange.index++;
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == THD_VALS && traverseNode->right_sibling->term == col) {
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subRangeCount = atoi(traverseNode->lexeme);
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subSubRanges = calloc(currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subRangeCount,sizeof(int));
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index = 0;
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == INT_VAR_LIST) {
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subSubRanges[currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index]++;
    }

    if(traverseNode->is_terminal && traverseNode->term == semicol && traverseNode->parent->non_term == INT_LIST_LIST_DASH) {
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index++;
    }

    if(traverseNode->is_terminal && traverseNode->term == cb_cl && traverseNode->parent->non_term == THD_VALS) {
        currTypeExpression->thdJaggedArrayRange.index++;
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
                    if(currTypeExpression->dataType == _prim) {
                        printf("%s ",currTypeExpression->typeName);
                    }
                    if(currTypeExpression->dataType == _array) {
                        printf("[ %s .. %s ] ",currTypeExpression->range_R1[0],currTypeExpression->range_R1[1]);
                        
                        if(currTypeExpression->dimensions>1) {
                            rect_array* currRectArr = currTypeExpression->rectArrayRanges->first;
                            do{
                                printf("[ %s .. %s ] ",currRectArr->range[0],currRectArr->range[1]);
                                currRectArr = currRectArr->next;
                            } while (currRectArr != NULL);
                        }
                        
                    }
                    if(currTypeExpression->dataType == _jagged) {
                        if(currTypeExpression->dimensions == 2) {
                            printf("range_R1= (%s,%s),",currTypeExpression->range_R1[0],currTypeExpression->range_R1[1]);
                            printf("range_R2=(");
                            for(int i=0;i<currTypeExpression->high-currTypeExpression->low;i++) {
                                printf("%s,",currTypeExpression->tdjaggedArrayRange.ranges[i]);
                            }
                            printf("%s)",currTypeExpression->tdjaggedArrayRange.ranges[currTypeExpression->high-currTypeExpression->low]);
                        }
                        if (currTypeExpression->dimensions == 3)
                        {
                            printf("range_R1= (%s,%s),",currTypeExpression->range_R1[0],currTypeExpression->range_R1[1]);
                            printf("range_R2=(");
                            for(int i=0;i<currTypeExpression->high-currTypeExpression->low;i++) {
                                printf("%d [ ",currTypeExpression->thdJaggedArrayRange.subRanges[i].subRangeCount);
                                for(int j=0;j<currTypeExpression->thdJaggedArrayRange.subRanges[i].subRangeCount-1;j++) {
                                    printf("%d,",currTypeExpression->thdJaggedArrayRange.subRanges[i].subSubRanges[j]);
                                }
                                printf("%d ],",currTypeExpression->thdJaggedArrayRange.subRanges[i].subSubRanges[currTypeExpression->thdJaggedArrayRange.subRanges[i].subRangeCount-1]);
                            }
                            printf("%d [ ",currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->high-currTypeExpression->low].subRangeCount);
                            for(int j=0;j<currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->high-currTypeExpression->low].subRangeCount-1;j++) {
                                printf("%d,",currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->high-currTypeExpression->low].subSubRanges[j]);
                            }
                            printf("%d ])",currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->high-currTypeExpression->low].subSubRanges[currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->high-currTypeExpression->low].subRangeCount-1]);

                        }
                        
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