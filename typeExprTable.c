#include "typeExprTable.h"
 //<type=sjkfnks>
            //LHS,not equaltype==
            // process assignment Type Checker
            //LHS Bool: Rhs has non bool var or Rhs has non bool operator
            //Array : All should have same typeExpression and no div operator
            //Int : Accessing element out of bounds in an array or any non integer variable or a division operator
            //Real : If integer is in expression then on left ther must be +/- operator and / after it and an integer after /.
                   //: Elements on both sides of / should be same. 
            //Find Type Expression of LHS
            //Compare it with all elements in RHS
            //Proper operators check
            //d=e/c;
            // ranges add 
            // operators add
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

bool initLHSAssign(void* id1, typeExpressionTable T, assignment_type_checker* checker, int type) {

    if(!type) {
        char* id2 = (char*) id1;
        assignExpression* temp = calloc(1,sizeof(assignExpression));
        temp->rangeNums = 0;
        temp->varName = calloc(1,MAX_VAR_NAME_LEN);
        strcpy(temp->varName,id2);
        temp->varType = calloc(1,sizeof(typeExpression));
        dataType* currVar = T->firstVariable;
        while (currVar!=NULL)
        {
            if(!strcmp(id2,currVar->varName)) {            
                temp->varType->typeName = calloc(1,strlen(currVar->type->typeName)+1);
                strcpy(temp->varType->typeName,currVar->type->typeName);
                temp->varType->dataType = currVar->type->dataType;
                temp->varType->arrayType = currVar->type->arrayType;
                temp->varType->primType = currVar->type->primType;
                // printf("%d\n",currVar->type->linenum);
                break;
            }
            currVar = currVar->next;
        }
        if(currVar == NULL) {
            temp->varType->dataType = _error;
            strcpy(temp->varType->typeName,"<type=ERROR>");
        }
        initLHSAssign(temp,T,checker,1);
        return currVar == NULL;
    }    
    
    checker->lhs = (assignExpression*) id1;
    return true;
}


bool computeTypeExprSummary(assignExpression* expr1, assignExpression* expr2, bool isDivop, bool isBoolOp ) {
    if(expr1->varType->dataType == _error) {
        free(expr2);
        return false;
    }
    if(expr2->varType->dataType == _error) {
        expr1->varType->dataType = _error;
        expr1->varType->typeName = calloc(1,14);
        strcpy(expr1->varType->typeName,"<type=ERROR>");
        return false;
    }

    if(isBoolOp) {
        free(expr2);
        return true;
    }

    if(strcmp(expr1->varType->typeName,expr2->varType->typeName)) {
        if(isDivop)
            {
                expr1->varType->primType = _real;
                strcpy(expr1->varType->typeName,"<type=real>");
            }
        free(expr2);
        return true;
    }
    if(!(expr1->varType->dataType == _prim && expr1->varType->primType == _real) && !(expr2->varType->dataType == _prim && expr2->varType->primType == _real) ) {
        expr1->varType->primType = _real;
        strcpy(expr1->varType->typeName,"<type=integer>");
        return true;
    }

    return false;
}

bool addTermToRHS(void* var, typeExpressionTable T, assignment_type_checker* checker, int type) {
    if(type == 0) {
        char* id = (char*) var;
        assignExpression* varToAdd = calloc(1,sizeof(assignExpression));
        varToAdd->varName = calloc(1,MAX_VAR_NAME_LEN);
        strcpy(varToAdd->varName,id);
        varToAdd->rangeNums = 0;
        varToAdd->rangeToFound = NULL;
        varToAdd->varType = calloc(1,sizeof(typeExpression));
        dataType* currVar = T->firstVariable;
        while (currVar!=NULL)
        {
            if(!strcmp(id,currVar->varName)) {            
                varToAdd->varType->typeName = calloc(1,strlen(currVar->type->typeName)+1);
                strcpy(varToAdd->varType->typeName,currVar->type->typeName);
                varToAdd->varType->dataType = currVar->type->dataType;
                varToAdd->varType->arrayType = currVar->type->arrayType;
                varToAdd->varType->primType = currVar->type->primType;
                // printf("%d\n",currVar->type->linenum);
                break;
            }
            currVar = currVar->next;
        }
        if(currVar == NULL) {
            varToAdd->varType->dataType = _error;
            varToAdd->varType->typeName = calloc(1,13);
            strcpy(varToAdd->varType->typeName,"<type=ERROR>");
        }
        addTermToRHS(varToAdd,T,checker,2);

    }
    if(type == 1) {
        //TODO add checker for num
        typeExpression* temp = calloc(1,sizeof(typeExpression));
        temp->dataType = _prim;
        temp->primType = _integer;
        temp->typeName = calloc(1,15);
        strcpy(temp->typeName,"<type=integer>");
        temp->arrayType = NA;
        assignExpression* tempAssExpr = calloc(1,sizeof(assignExpression));
        tempAssExpr->varType = temp;
        tempAssExpr->rangeNums = 0;
        tempAssExpr->rangeToFound = NULL;
        tempAssExpr->varName = NULL;
        addTermToRHS(tempAssExpr,T,checker,2);
    }
    if(type == 2) {
        assignExpression* incomingVar = (assignExpression*) var;
        if(checker->lRHS == NULL) {
            checker->lRHS = incomingVar;
            return true;
        }
        if(checker->rRHS == NULL) {
            if(checker->hasMDAop) {
                bool ret = computeTypeExprSummary(checker->lRHS,incomingVar, checker->hasDivop,checker->hasBoolop);
                incomingVar = NULL;
                checker->hasMDAop = false;
                checker->hasDivop = false;
                return ret;
            }
            if(checker->hasASOop[0]) {
                checker->rRHS = incomingVar;
            }
            return true;
        }
        bool ret;
        if(checker->hasMDAop && checker->hasASOop[0]) {
            ret = computeTypeExprSummary(checker->rRHS,incomingVar, checker->hasDivop, checker->hasASOop);
            incomingVar = NULL;
            checker->hasMDAop = false;
            checker->hasDivop = false;
        }
        else {
            ret = computeTypeExprSummary(checker->lRHS,checker->rRHS, checker->hasDivop, checker->hasASOop);
            checker->rRHS = incomingVar;
        }

        return ret;
        
        
    }
    
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
        BUF = calloc(1,5);
        sprintf(BUF,"%d,",t->tdJaggedArrayRange.ranges[i][0]);
        strcat(expr,BUF);
        free(BUF);
    }
    BUF = calloc(1,5);
    sprintf(BUF,"%d",t->tdJaggedArrayRange.ranges[t->high-t->low][0]);
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
void computePrimtypeExpr(typeExpression* typeExpr,terminal term) {
    char *type  = calloc(1,15);
    switch (term)
    {
    case integer:
        typeExpr->primType = _integer;
        strcpy(type,"<type=integer>");
        break;
    case real:
        typeExpr->primType = _real;
        strcpy(type,"<type=real>");
        break;
    case boolean:
        typeExpr->primType = _boolean;
        strcpy(type,"<type=boolean>");
        break;
    }
    typeExpr->typeName = type;
}

void traverseParseTree(parseTree *t, typeExpressionTable T) {
    parseTree* traverseNode = t;
    dataType* currentVariable = T->firstVariable;
    int flag = 0;
    stack * mainStack = create_stack();
    typeExpression* currTypeExpression;
    assignExpression* varToAdd = NULL;
    assignment_type_checker* assignmentTypeChecker = NULL;
    int* arrayCheck = NULL;
    bool isMultID = false;
    bool isAssignStatement = false;
do
{
    traverseNode->currNode = traverseNode->left_most_child;
    
    if(!traverseNode->is_terminal && traverseNode->non_term == DECLARE_STATEMENT) {
        currTypeExpression = traverseNode->type;    //init new type expr
        if(arrayCheck!=NULL)
            free(arrayCheck);
        arrayCheck = NULL;
        isMultID = false;
    }
    
    if(!traverseNode->is_terminal && traverseNode->non_term == ASSIGN_STATEMENT) {
        if(assignmentTypeChecker!= NULL)
        {
           
            if(assignmentTypeChecker->lhs->varType->dataType == _error)
            {
                if(assignmentTypeChecker->rRHS !=NULL)
                    assignmentTypeChecker->rRHS->varType->dataType = _error;
                    if(assignmentTypeChecker->lRHS !=NULL)
                    assignmentTypeChecker->lRHS->varType->dataType = _error;
            }
            
            if(flag)
                printf("Type Error at line : %d\n",assignmentTypeChecker->linenum);
            free(assignmentTypeChecker);
        }
        assignmentTypeChecker = NULL;
        assignmentTypeChecker = calloc(1,sizeof(assignment_type_checker));
        assignmentTypeChecker->hasASOop[0] = false;
        assignmentTypeChecker->hasASOop[1] = false;
        assignmentTypeChecker->hasBoolop = false;
        assignmentTypeChecker->hasMDAop = false;
        assignmentTypeChecker->hasDivop = false;
        assignmentTypeChecker->lhs = NULL;
        assignmentTypeChecker->rRHS = NULL;
        assignmentTypeChecker->lRHS = NULL;
        currTypeExpression = calloc(1,sizeof(dataType));    //init new type expr
        if(arrayCheck!=NULL)
            free(arrayCheck);
        arrayCheck = NULL;
        isMultID = false;
        isAssignStatement = true;
    }
    
    if(traverseNode->is_terminal && traverseNode->term == id && traverseNode->parent->non_term == ID1 && traverseNode->parent->parent->non_term == ASSIGN_STATEMENT) {
        if(!traverseNode->right_sibling->left_most_child->is_terminal) {
            bool added = initLHSAssign(traverseNode->lexeme,T,assignmentTypeChecker,0);
            assignmentTypeChecker->linenum = traverseNode->linenum;
            if(assignmentTypeChecker->lhs->varType->dataType == _error) {
                printf("Type error at line %d, ", traverseNode->linenum);
                added?printf("variable %s has erroneous type\n",traverseNode->lexeme):printf("variable %s not found\n",traverseNode->lexeme);
                currTypeExpression = assignmentTypeChecker->lhs->varType;
            }
        }
        else {
            varToAdd = calloc(1,sizeof(assignExpression));
            varToAdd->varName = calloc(1,MAX_VAR_NAME_LEN);
            strcpy(varToAdd->varName,traverseNode->lexeme);
            varToAdd->rangeNums = 0;
            varToAdd->rangeToFound = NULL;
        } 
    }
    // if(traverseNode->is_terminal && traverseNode->term == id && traverseNode->parent->non_term == IDX && traverseNode->parent->parent->non_term == ID_DASH) {
    //     varToAdd->rangeNums++;
    //     if(varToAdd->rangeToFound == NULL)
    //         varToAdd->rangeToFound = calloc(1,sizeof(int));
    //     else
    //         varToAdd->rangeToFound = realloc(varToAdd->rangeToFound,varToAdd->rangeNums*sizeof(int));      
    // }

    if(traverseNode->is_terminal && traverseNode->term == id && traverseNode->parent->non_term == ID1 && traverseNode->parent->parent->non_term == IDX1 && (traverseNode->parent->parent->parent->non_term == TERM || traverseNode->parent->parent->parent->non_term == TERM_DASH)) {
        
        addTermToRHS(traverseNode->lexeme,T,assignmentTypeChecker,0);
    }
    if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == IDX1 && (traverseNode->parent->parent->non_term == TERM || traverseNode->parent->parent->non_term == TERM_DASH)) {
        addTermToRHS(traverseNode->lexeme,T,assignmentTypeChecker,1);
    }

    if(traverseNode->is_terminal && traverseNode->term == add_op) {
        if(!assignmentTypeChecker->hasBoolop) {
            // It is Cool too
            // lRHS and rRHS are boolean case not considered
            if(assignmentTypeChecker->rRHS == NULL) {
                assignmentTypeChecker->hasASOop[0] = true;
            }
            else
            {
                computeTypeExprSummary(assignmentTypeChecker->lRHS,assignmentTypeChecker->rRHS,assignmentTypeChecker->hasDivop,assignmentTypeChecker->hasBoolop);
                assignmentTypeChecker->rRHS = NULL;           
            }
            
        }
        else
        {
            // It is fine
            computeTypeExprSummary(assignmentTypeChecker->lRHS,assignmentTypeChecker->rRHS,assignmentTypeChecker->hasDivop,assignmentTypeChecker->hasBoolop);
            assignmentTypeChecker->rRHS = NULL;
            currTypeExpression->dataType = _error;
            currTypeExpression->typeName = calloc(1,13);
            strcpy(currTypeExpression->typeName,"<type=ERROR>");
            assignmentTypeChecker->hasBoolop = false;
        }
    }
    if(traverseNode->is_terminal && traverseNode->term == sub_op) {
        // It is Cool too
        // lRHS and rRHS are boolean case not considered
        if(!assignmentTypeChecker->hasBoolop) {
            if(assignmentTypeChecker->rRHS == NULL) {
                assignmentTypeChecker->hasASOop[0] = true;
            }
            else
            {
                computeTypeExprSummary(assignmentTypeChecker->lRHS,assignmentTypeChecker->rRHS,assignmentTypeChecker->hasDivop,assignmentTypeChecker->hasBoolop);
                assignmentTypeChecker->rRHS = NULL;           
            }
            
        }
        else
        {
            computeTypeExprSummary(assignmentTypeChecker->lRHS,assignmentTypeChecker->rRHS,assignmentTypeChecker->hasDivop,assignmentTypeChecker->hasBoolop);
            assignmentTypeChecker->rRHS = NULL;
            currTypeExpression->dataType = _error;
            currTypeExpression->typeName = calloc(1,13);
            strcpy(currTypeExpression->typeName,"<type=ERROR>");
            assignmentTypeChecker->hasBoolop = false;
        }
    }
    if(traverseNode->is_terminal && traverseNode->term == mul_op) {

        // Suppose lRHS is boolean and rRHS is NULL not Checked.
        if(assignmentTypeChecker->rRHS!=NULL && assignmentTypeChecker->rRHS->varType->dataType == _prim && assignmentTypeChecker->rRHS->varType->primType == _boolean) {
            currTypeExpression->dataType = _error;
            currTypeExpression->typeName = calloc(1,13);
            strcpy(currTypeExpression->typeName,"<type=ERROR>");          
        }
        assignmentTypeChecker->hasMDAop = true;
    }
    if(traverseNode->is_terminal && traverseNode->term == div_op) {
        // Suppose lRHS is boolean and rRHS is NULL not Checked.
        if(assignmentTypeChecker->rRHS!=NULL && assignmentTypeChecker->rRHS->varType->dataType == _prim && assignmentTypeChecker->rRHS->varType->primType == _boolean) {
            currTypeExpression->dataType = _error;
            currTypeExpression->typeName = calloc(1,13);
            strcpy(currTypeExpression->typeName,"<type=ERROR>");          
        }
        assignmentTypeChecker->hasMDAop = true;
        assignmentTypeChecker->hasDivop = true;     
    }
    if(traverseNode->is_terminal && traverseNode->term == or_op) {
        if(assignmentTypeChecker->hasBoolop && assignmentTypeChecker->hasASOop[0]) {
            computeTypeExprSummary(assignmentTypeChecker->lRHS,assignmentTypeChecker->rRHS,assignmentTypeChecker->hasDivop,assignmentTypeChecker->hasBoolop);
            assignmentTypeChecker->rRHS = NULL;
        }
        else
        {
            if(assignmentTypeChecker->rRHS != NULL) { // Because rRHS is not NULL and BoolOp is false
                currTypeExpression->dataType = _error;
                currTypeExpression->typeName = calloc(1,13);
                strcpy(currTypeExpression->typeName,"<type=ERROR>");
            }
            else if(!(assignmentTypeChecker->lRHS->varType->dataType == _prim && assignmentTypeChecker->lRHS->varType->primType == _boolean)) {
                currTypeExpression->dataType = _error;
                currTypeExpression->typeName = calloc(1,13);
                strcpy(currTypeExpression->typeName,"<type=ERROR>");
            }         
            assignmentTypeChecker->hasBoolop = true;
            // ASOP True kyu nahi kiya
        }       
    }
    if(traverseNode->is_terminal && traverseNode->term == and_op) {
        assignmentTypeChecker->hasMDAop = true;
        assignmentTypeChecker->hasBoolop = true;
        if(assignmentTypeChecker->rRHS != NULL && !(assignmentTypeChecker->rRHS->varType->dataType == _prim && assignmentTypeChecker->rRHS->varType->primType == _boolean)) {
            currTypeExpression->dataType = _error;
            currTypeExpression->typeName = calloc(1,13);
            strcpy(currTypeExpression->typeName,"<type=ERROR>");
        }
        else if (assignmentTypeChecker->rRHS == NULL && !(assignmentTypeChecker->lRHS->varType->dataType == _prim && assignmentTypeChecker->lRHS->varType->primType == _boolean))
        {
            currTypeExpression->dataType = _error;
            currTypeExpression->typeName = calloc(1,13);
            strcpy(currTypeExpression->typeName,"<type=ERROR>");
        }       
    }
    if(traverseNode->is_terminal && traverseNode->term == semicol && traverseNode->parent->non_term == ASSIGN_STATEMENT) {
        if(assignmentTypeChecker->rRHS != NULL) {
            computeTypeExprSummary(assignmentTypeChecker->lRHS,assignmentTypeChecker->rRHS,assignmentTypeChecker->hasDivop,assignmentTypeChecker->hasBoolop);
        }
        computeTypeExprSummary(assignmentTypeChecker->lhs,assignmentTypeChecker->lRHS,false,false);
        currTypeExpression->dataType = assignmentTypeChecker->lhs->varType->dataType;
        currTypeExpression->primType = assignmentTypeChecker->lhs->varType->primType;
        if(assignmentTypeChecker->lhs->varType->dataType == _error) {
            printf("type error at line : %d\n", traverseNode->linenum);
        }
        else
        {
            printf("line %d is ok\n",traverseNode->linenum);
        }
        
    }
    {
        if(traverseNode->parent !=NULL) {
            if(traverseNode->is_terminal && traverseNode->parent->non_term == PRIM_TYPE) {
                if(currTypeExpression->dataType != _error)
                { 
                    currTypeExpression->dataType = _prim;
                    computePrimtypeExpr(currTypeExpression,traverseNode->term);
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
        if(!traverseNode->is_terminal && traverseNode->non_term == VAR_LIST) {
            isMultID = true;
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
                arrayCheck = calloc(currTypeExpression->high - currTypeExpression->low + 1,sizeof(int));
            }

        }

        if(traverseNode->is_terminal && traverseNode->term == of && traverseNode->parent->non_term == TD_INIT && currTypeExpression->dataType != _error) {
            currTypeExpression->tdJaggedArrayRange.ranges = calloc(currTypeExpression->high-currTypeExpression->low+1,sizeof(int*));
            for(int i=0;i<currTypeExpression->high-currTypeExpression->low+1;i++) {
                currTypeExpression->tdJaggedArrayRange.ranges[i] = calloc(2,sizeof(int));
            }
            currTypeExpression->tdJaggedArrayRange.index = 0;
        }

        if(traverseNode->is_terminal && traverseNode->term == sq_op && traverseNode->parent->non_term == THD_INIT && currTypeExpression->dataType != _error) {
            currTypeExpression->dimensions = 3;
            currTypeExpression->thdJaggedArrayRange.subRanges = calloc(currTypeExpression->high-currTypeExpression->low+1,sizeof(thd_sub_range));
            currTypeExpression->thdJaggedArrayRange.index = 0;
        }

        if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == TD_VALS && traverseNode->right_sibling->term == col && currTypeExpression->dataType != _error) {
            currTypeExpression->tdJaggedArrayRange.ranges[currTypeExpression->tdJaggedArrayRange.index][0] = atoi(traverseNode->lexeme);
            currTypeExpression->tdJaggedArrayRange.ranges[currTypeExpression->tdJaggedArrayRange.index][1] = 0;
        }

        if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == INT_LIST && currTypeExpression->dataType!=_error) {
            currTypeExpression->tdJaggedArrayRange.ranges[currTypeExpression->tdJaggedArrayRange.index][1]++;
        }

        if(traverseNode->is_terminal && traverseNode->term == cb_cl && traverseNode->parent->non_term == TD_VALS && currTypeExpression->dataType!=_error) {
            if(currTypeExpression->tdJaggedArrayRange.ranges[currTypeExpression->tdJaggedArrayRange.index][0] != currTypeExpression->tdJaggedArrayRange.ranges[currTypeExpression->tdJaggedArrayRange.index][1]){
                printf("%d %d\n",currTypeExpression->tdJaggedArrayRange.ranges[currTypeExpression->tdJaggedArrayRange.index][0],currTypeExpression->tdJaggedArrayRange.ranges[currTypeExpression->tdJaggedArrayRange.index][1]);
                currTypeExpression->dataType = _error;
                printf("Type definition error at line %d, sub-range does not match in 2D jagged array\n", traverseNode->linenum);
                currTypeExpression->linenum = traverseNode->linenum;
            }
            currTypeExpression->tdJaggedArrayRange.index++;
        }

        if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == TD_VALS && traverseNode->right_sibling->term == sq_cl && currTypeExpression->dataType != _error) {
            int number = atoi(traverseNode->lexeme);
            if(number<currTypeExpression->low || number>currTypeExpression->high) {
                currTypeExpression->dataType = _error;
                printf("Type definition error at line %d, index out of bounds in 2D jagged array\n", traverseNode->linenum);
                currTypeExpression->linenum = traverseNode->linenum;
            }
            else if(arrayCheck[number-currTypeExpression->low]){
                currTypeExpression->dataType = _error;
                printf("Type definition error at line %d, multiple declarations of same index in 2D jagged array\n", traverseNode->linenum);     
                currTypeExpression->linenum = traverseNode->linenum;
            }
            else
            {
                arrayCheck[number-currTypeExpression->low] = true;
            }
            
        }

        if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == THD_VALS && traverseNode->right_sibling->term == sq_cl && currTypeExpression->dataType != _error) {
            int number = atoi(traverseNode->lexeme);
            // printf("%d \n",number);
            if(number<currTypeExpression->low || number>currTypeExpression->high) {
                currTypeExpression->dataType = _error;
                printf("Type definition error at line %d, index out of bounds in 3D jagged array\n", traverseNode->linenum);
                currTypeExpression->linenum = traverseNode->linenum;
            }
            else if(arrayCheck[number-currTypeExpression->low]){
                currTypeExpression->dataType = _error;
                printf("Type definition error at line %d, multiple declarations of same index in 3D jagged array\n", traverseNode->linenum);     
                currTypeExpression->linenum = traverseNode->linenum;
            }
            else
            {
                arrayCheck[number-currTypeExpression->low] = true;
            }
        }

        if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == THD_VALS && traverseNode->right_sibling->term == col && currTypeExpression->dataType != _error) {
            currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subRangeCount = atoi(traverseNode->lexeme);
            currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subSubRanges = calloc(currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subRangeCount,sizeof(int));
            currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index = 0;
        }

        if(traverseNode->is_terminal && traverseNode->term == num && traverseNode->parent->non_term == INT_VAR_LIST_DASH && currTypeExpression->dataType != _error) {
            currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subSubRanges[currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index]++;
        }

        if(traverseNode->is_terminal && traverseNode->term == semicol && traverseNode->parent->non_term == INT_LIST_LIST_DASH && currTypeExpression->dataType != _error) {
            currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index++;
            if(currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].subSubRanges[currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index].index-1] == 0) {
                currTypeExpression->dataType = _error;
                printf("Type definition erorr at line %d, sub-sub-range size cannot be zero\n",traverseNode->linenum);
                currTypeExpression->linenum = traverseNode->linenum;
            }
        }

        if(traverseNode->is_terminal && traverseNode->term == cb_cl && traverseNode->parent->non_term == THD_VALS && currTypeExpression->dataType != _error) {
            currTypeExpression->thdJaggedArrayRange.index++;
            if(currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index-1].subRangeCount != currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index-1].index + 1) {
                currTypeExpression->dataType = _error;
                printf("Type definition error at line %d, sub-sub-range size does not match in 3D jagged array\n ", traverseNode->linenum);
                currTypeExpression->linenum = traverseNode->linenum;
            }
            if(currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index-1].subSubRanges[currTypeExpression->thdJaggedArrayRange.subRanges[currTypeExpression->thdJaggedArrayRange.index-1].index] == 0) {
                currTypeExpression->dataType = _error;
                printf("Type definition erorr at line %d, sub-sub-range size cannot be zero\n",traverseNode->linenum);
                currTypeExpression->linenum = traverseNode->linenum;
            }
        }
        if(traverseNode->is_terminal && traverseNode->term == cb_cl && traverseNode->right_sibling!=NULL && !traverseNode->right_sibling->is_terminal) {
            if(traverseNode->right_sibling->left_most_child->non_term == EPS) {
                if(traverseNode->right_sibling->non_term == TD_VALS_DASH) {
                    if(currTypeExpression->high-currTypeExpression->low+1> currTypeExpression->tdJaggedArrayRange.index) {
                        printf("Type definition error at line %d, sub-range size does not match in 2D jagged array\n", traverseNode->linenum);
                    } 
                }
                if(traverseNode->right_sibling->non_term == THD_VALS_DASH) {
                    if(currTypeExpression->high-currTypeExpression->low+1> currTypeExpression->thdJaggedArrayRange.index) {
                        printf("Type definition error at line %d, sub-range size does not match in 3D jagged array\n", traverseNode->linenum);
                    } 
                }
            }
        }
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
                if(isMultID && mainStack->top == 0){
                   printf("Warning at line %d, too few variables in multi declare statement\n", mainStack->arr[mainStack->top].linenum); 
                }
                
                while(mainStack->top!=-1) {
                    parseTree currID = pop(mainStack);
                    currID.type = currTypeExpression;
                    currID.parent->type = currTypeExpression;
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
}

void printTypeExpressionTable (typeExpressionTable T){
    dataType* currVariable = T->firstVariable;
    int i=0;
    while (currVariable!=NULL)
    {
        if(currVariable->type!=NULL)
            printf("%s %s ",currVariable->varName, currVariable->type->typeName);
            if(currVariable->type->dataType == _error) {
                printf("Type definition error at Line No. : %d",currVariable->type->linenum);
            }
            printf("\n");
        currVariable = currVariable->next;
    }
    
}
//IDX ASOP IDX MDAOP
// a= b + c + d /e