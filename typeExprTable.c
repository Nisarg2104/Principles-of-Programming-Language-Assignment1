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
void compute2DJaggedArraytypeExpr(typeExpression* t)
{
    char* expr = malloc(256);
    strcpy(expr,"<type=jaggedArray,dimensions=");
    char num [3];
    sprintf(num,"%d",t->dimensions);
    strcat(expr, num);
    strcat(expr,",range_R1=(");
    strcat(expr,t->range_R1[0]);
    strcat(expr,",");
    strcat(expr,t->range_R1[1]);
    strcat(expr,"),range_R2=(");
    char* BUF;
    for(int i = 0;i<t->high-t->low;i++) {
        BUF = calloc(1,30);
        sprintf(BUF,"%s,",t->tdjaggedArrayRange.ranges[i]);
        strcat(expr,BUF);
        free(BUF);
    }
    BUF = calloc(1,5);
    sprintf(BUF,"%s",t->tdjaggedArrayRange.ranges[t->high-t->low]);
    strcat(expr,BUF);
    free(BUF);
    strcat(expr,"),basicElementType = integer>");
    t->typeName = expr;
}
void compute3DJaggedArraytypeExpr(typeExpression* t) {
    char*expr = calloc(1,256);
    sprintf(expr,"<type=jaggedArray,dimensions=3,range_R1= (%s,%s),",t->range_R1[0],t->range_R1[1]);
    strcat(expr,"range_R2=(");
    char* BUF;
    for(int i=0;i<t->high-t->low;i++) {
        BUF = calloc(1,50);
        sprintf(BUF,"%d [ ",t->thdJaggedArrayRange.subRanges[i].subRangeCount);
        strcat(expr,BUF);
        free(BUF);
        for(int j=0;j<t->thdJaggedArrayRange.subRanges[i].subRangeCount-1;j++) {
            BUF = calloc(1,5);
            sprintf(BUF,"%d,",t->thdJaggedArrayRange.subRanges[i].subSubRanges[j]);
            strcat(expr,BUF);
            free(BUF);
        }
        BUF = calloc(1,8);
        sprintf(BUF,"%d ],",t->thdJaggedArrayRange.subRanges[i].subSubRanges[t->thdJaggedArrayRange.subRanges[i].subRangeCount-1]);
        strcat(expr,BUF);
        free(BUF);
    }
    BUF = calloc(1,8);
    sprintf(BUF,"%d [ ",t->thdJaggedArrayRange.subRanges[t->high-t->low].subRangeCount);
    strcat(expr,BUF);
    free(BUF);
    for(int j=0;j<t->thdJaggedArrayRange.subRanges[t->high-t->low].subRangeCount-1;j++) {
        BUF = calloc(1,6);
        sprintf(BUF,"%d,",t->thdJaggedArrayRange.subRanges[t->high-t->low].subSubRanges[j]);
        strcat(expr,BUF);
        free(BUF);
    }
    BUF = calloc(1,8);
    sprintf(BUF,"%d ])",t->thdJaggedArrayRange.subRanges[t->high-t->low].subSubRanges[t->thdJaggedArrayRange.subRanges[t->high-t->low].subRangeCount-1]);
    strcat(expr,BUF);
    strcat(expr,",basicElementType = integer>");
    t->typeName = expr;
}
void computeRegularArraytypeExpr(typeExpression* t) {
    char* expr = malloc(256);
    strcpy(expr,"<type=rectangularArray,dimensions=");
    char num [3];
    sprintf(num,"%d",t->dimensions);
    strcat(expr, num);
    strcat(expr,",range_R1=(");
    strcat(expr,t->range_R1[0]);
    strcat(expr,",");
    strcat(expr,t->range_R1[1]);
    strcat(expr,")");

    if(t->dimensions>1) {
        rect_array* currRectArr = t->rectArrayRanges->first;
        char* BUF;
        int i=2;
        do{
            BUF = calloc(1,30);
            sprintf(BUF,",range_R%d=(%s,%s)",i,currRectArr->range[0],currRectArr->range[1]);
            currRectArr = currRectArr->next;
            strcat(expr,BUF);
            free(BUF);
            i++;
        } while (currRectArr != NULL);
    }
    strcat(expr,",basicElementType = integer>");
    t->typeName = expr;
}

void traverseParseTree(parseTree *t, typeExpressionTable T) {
    parseTree* traverseNode = t;
    dataType* currentVariable = T->firstVariable;
    int flag = 0;
    stack * mainStack = create_stack();
    typeExpression* currTypeExpression;
do
{
    traverseNode->currNode = traverseNode->left_most_child;
    
    if(!traverseNode->is_terminal && traverseNode->non_term == DECLARE_STATEMENT) {
        currTypeExpression = traverseNode->type;    //init new type expr
    }
    if(traverseNode->parent !=NULL) {
        if(traverseNode->is_terminal && traverseNode->parent->non_term == PRIM_TYPE) {
            if(currTypeExpression->dataType != _error)
            { 
                currTypeExpression->dataType = _prim;
                currTypeExpression->typeName=returnPrimitiveType(traverseNode->term);
                currTypeExpression->arrayType = NA;}
            }
        if(!traverseNode->is_terminal && traverseNode->parent->non_term == ARRAY) {
            if(traverseNode->non_term == JAGGED) {
                if(currTypeExpression->dataType != _error) {
                    currTypeExpression->dataType = _jagged;
                    currTypeExpression->arrayType = NA;
                    currTypeExpression->dimensions = 2;
                }
            }
            else
            {
                if(currTypeExpression->dataType != _error)
                {
                    currTypeExpression->dataType = _array;
                    currTypeExpression->dimensions = 0;
                }
            }
            
        }
    }
    

    if(traverseNode->is_terminal && traverseNode->term == id && (traverseNode->parent->non_term == MULT_ID || traverseNode->parent->non_term == VAR_LIST || traverseNode->parent->non_term == SINGLE_DECLARE)) {
        push(mainStack,*traverseNode);
    }

    if(traverseNode->is_terminal && traverseNode->term == id && traverseNode->parent->non_term == IDX && traverseNode->parent->right_sibling->is_terminal && (traverseNode->parent->right_sibling->term == ddot || traverseNode->parent->right_sibling->term == sq_cl)) {
        if(currTypeExpression->dimensions == 0) {
            if(traverseNode->parent->right_sibling->term == ddot && currTypeExpression->dataType != _error) {
                currTypeExpression->range_R1[0] = traverseNode->lexeme;
            }
            else if(currTypeExpression->dataType != _error) {
                currTypeExpression->range_R1[1] = traverseNode->lexeme;
                currTypeExpression->dimensions++;
            }
        }
        else {
            if(currTypeExpression->dimensions == 1) {
                
                if(traverseNode->parent->right_sibling->term == ddot && currTypeExpression->dataType != _error) {
                    currTypeExpression->rectArrayRanges = calloc(1,sizeof(rect_array_ranges));
                    currTypeExpression->rectArrayRanges->first = calloc(1,sizeof(rect_array));
                    currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->first;
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else if(currTypeExpression->dataType != _error){
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                    currTypeExpression->dimensions++;
                }
            }
            else {

                if(traverseNode->parent->right_sibling->term == ddot && currTypeExpression->dataType != _error) {
                    currTypeExpression->rectArrayRanges->last->next = calloc(1,sizeof(rect_array));
                    currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->last->next;
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else if(currTypeExpression->dataType != _error){
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                    currTypeExpression->dimensions++;
                }
            }
            
        }
    }
    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == IDX && traverseNode->parent->right_sibling->is_terminal && (traverseNode->parent->right_sibling->term == ddot || traverseNode->parent->right_sibling->term == sq_cl)) {
        if(currTypeExpression->dimensions == 0) {
            if(traverseNode->parent->right_sibling->term == ddot && currTypeExpression->dataType != _error) {
                currTypeExpression->range_R1[0] = traverseNode->lexeme;
            }
            else if(currTypeExpression->dataType != _error) {
                currTypeExpression->range_R1[1] = traverseNode->lexeme;
                currTypeExpression->dimensions++;
            }
        }
        else {
            if(currTypeExpression->dimensions == 1) {
                
                if(traverseNode->parent->right_sibling->term == ddot && currTypeExpression->dataType != _error) {
                    currTypeExpression->rectArrayRanges = calloc(1,sizeof(rect_array_ranges));
                    currTypeExpression->rectArrayRanges->first = calloc(1,sizeof(rect_array));
                    currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->first;
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else if(currTypeExpression->dataType != _error){
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                    currTypeExpression->dimensions++;
                }
            }
            else {

                if(traverseNode->parent->right_sibling->term == ddot && currTypeExpression->dataType != _error) {
                    currTypeExpression->rectArrayRanges->last->next = calloc(1,sizeof(rect_array));
                    currTypeExpression->rectArrayRanges->last = currTypeExpression->rectArrayRanges->last->next;
                    currTypeExpression->rectArrayRanges->last->range[0] = traverseNode->lexeme;
                }
                else if(currTypeExpression->dataType != _error) {
                    currTypeExpression->rectArrayRanges->last->range[1] = traverseNode->lexeme;
                    currTypeExpression->dimensions++;
                }
            }
            
        }
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == JAGGED) {
        if(traverseNode->right_sibling->term == ddot && currTypeExpression->dataType != _error) {

            currTypeExpression->range_R1[0] = traverseNode->lexeme;
            currTypeExpression->low = atoi(traverseNode->lexeme);
        }
        else if(currTypeExpression->dataType != _error){
            currTypeExpression->range_R1[1] = traverseNode->lexeme;
            currTypeExpression->high = atoi(traverseNode->lexeme);
        }
    }

    if(traverseNode->is_terminal && traverseNode->term == of && traverseNode->parent->non_term == TD_INIT && currTypeExpression->dataType != _error) {
        currTypeExpression->tdjaggedArrayRange.ranges = calloc(currTypeExpression->high-currTypeExpression->low+1,sizeof(char*));
        currTypeExpression->tdjaggedArrayRange.index = 0;
    }

    if(traverseNode->is_terminal && traverseNode->term == sq_op && traverseNode->parent->non_term == THD_INIT && currTypeExpression->dataType != _error) {
        currTypeExpression->dimensions = 3;
        currTypeExpression->thdJaggedArrayRange.subRanges = calloc(currTypeExpression->high-currTypeExpression->low+1,sizeof(thd_sub_range));
        currTypeExpression->thdJaggedArrayRange.index = 0;
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == TD_VALS && traverseNode->right_sibling->term == col && currTypeExpression->dataType != _error) {
        currTypeExpression->tdjaggedArrayRange.ranges[currTypeExpression->tdjaggedArrayRange.index] = traverseNode->lexeme;
        currTypeExpression->tdjaggedArrayRange.index++;
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == THD_VALS && traverseNode->right_sibling->term == col && currTypeExpression->dataType != _error) {
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subRangeCount = atoi(traverseNode->lexeme);
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subSubRanges = calloc(currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subRangeCount,sizeof(int));
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index = 0;
    }

    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == INT_VAR_LIST && currTypeExpression->dataType != _error) {
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subSubRanges[currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index]++;
    }

    if(traverseNode->is_terminal && traverseNode->term == semicol && traverseNode->parent->non_term == INT_LIST_LIST_DASH && currTypeExpression->dataType != _error) {
        currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index++;
    }

    if(traverseNode->is_terminal && traverseNode->term == cb_cl && traverseNode->parent->non_term == THD_VALS && currTypeExpression->dataType != _error) {
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
                if(mainStack->top == -1) {
                    continue;
                }
                if(currTypeExpression->dataType == _error) {
                    currTypeExpression->typeName = calloc(1,13);
                    strcpy(currTypeExpression->typeName,"<type=ERROR>");
                }
                else if(currTypeExpression->dataType == _array) {
                    computeRegularArraytypeExpr(currTypeExpression);
                }
                else if (currTypeExpression->dataType ==_jagged)
                {
                    if(currTypeExpression->dimensions == 2) {
                        compute2DJaggedArraytypeExpr(currTypeExpression);
                    }
                    else
                    {
                        compute3DJaggedArraytypeExpr(currTypeExpression);
                    }
                    
                }
                
                while(mainStack->top!=-1) {
                    parseTree currID = pop(mainStack);
                    currID.type = currTypeExpression;
                    // printf("%s %s\n",currID.lexeme,currTypeExpression->typeName);

                    if(T->firstVariable==NULL) {
                        T->firstVariable =(dataType*) calloc(1,sizeof(dataType));
                        currentVariable = T->firstVariable;
                        T->variables = 1;
                    }
                    else {
                        T->variables++;
                        currentVariable->next = (dataType*)calloc(1,sizeof(dataType));
                        currentVariable = currentVariable->next;
                    }
                    currentVariable->type = currTypeExpression;
                    currentVariable->varName = (char*)calloc(1,MAX_VAR_NAME_LEN);
                    strcpy(currentVariable->varName,currID.lexeme);
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
    printTypeExpressionTable(T);
}

void printTypeExpressionTable (typeExpressionTable T){
    printf("%d\n",T->variables);
    dataType* currVariable = T->firstVariable;
    int i=0;
    while (currVariable!=NULL)
    {
        if(currVariable->type!=NULL)
            printf("%s %s\n",currVariable->varName, currVariable->type->typeName);
        currVariable = currVariable->next;
    }
    
}