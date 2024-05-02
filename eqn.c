// #pragma once
#include "eqn.h"

eqnNode* NewDefaultEqnNode()
{
    eqnNode* res = NEW_EQN_NODE();
    res->sign = 0;
    res->poly1 = NULL;
    res->shouldBeFreed = 0;
    res->next = NULL;
    return res;
}

void deleteEqnLkList(eqnNode* node)
{
    eqnNode* next;
    while(node)
    {
        // printf("del node: %p  %p\n", node, node->next);
        next = node->next;
        if(node->shouldBeFreed)
        {
            // For Debugging
            // printf("Deleted: "); displayPrPoly(node->poly1);
            DeletePoly(node->poly1);
        }
        free(node);
        node = next;
    }
}

poly* solveEqn(char *str, pDBNode* DBHead, int* pCharsRead, char ShouldPrintError)
{
    // in this function, some polys are taken from database, 
    // some are newly generated as intermediate results, these should be freed after calculating final result
    // these are marks with flag shouldBeFreed;

    // charsRead - no. of characters read by the function from str, useful while recursion

    eqnNode* Head = NewDefaultEqnNode();
    eqnNode* body = Head;
    char first_term = 1;

    char sign, shouldBeFreed;
    int lidx = 0, ridx = 0; // left index , right index
    poly* poly1;
    char shouldNegatePoly = 0; // a flag that says whether to negate the poly or not in the current iteration

    while(str[lidx]==' ') lidx++;

    if(str[lidx]=='*') {
        if (ShouldPrintError) {
            printf("Error occurred while parsing expression:\n");
            printf("    %s\n", str);
            for (int i = 0; i < 4+lidx; i++) printf(" ");
            printf("^\n");
        }
        deleteEqnLkList(Head);
        return NULL;
    }

    while(str[ridx])
    {
        poly1 = NULL;
        shouldBeFreed = 0;
        shouldNegatePoly = 0;

        while(str[lidx]==' ') lidx++; // ignoring front spaces
        if (str[lidx]==0 || str[lidx]==')') {
            lidx++;
            ridx = lidx;
            break;
        }

        if (str[lidx]=='+') {sign = 1; lidx++;}
        else if (str[lidx] == '-') {sign = -1; lidx++;}
        else if (str[lidx] == '*') {
            sign = 2;
            lidx++;
            // following logic is to read " *- " without any error
            while(str[lidx]==' ') lidx++; // ignoring spaces
            if (str[lidx] == '-') {
                shouldNegatePoly = 1;
                lidx++;
            }
        }
        else if (first_term) {
            sign = 1;
            first_term = 0;
        }
        else {
            // ! Error
            if (ShouldPrintError) {
                printf("Error occurred while parsing expression:\n");
                printf("    %s\n", str);
                for (int i = 0; i < 4+lidx; i++) printf(" ");
                printf("^\n");
            }
            deleteEqnLkList(Head);
            return NULL;
        }

        while(str[lidx]==' ') lidx++; // ignoring the spaces after sign
        
        ridx = lidx; // sign is read, now the next part

        // if there are brackets --> recursion
        if (str[lidx]=='(')
        {
            lidx++;
            int charsRead;
            poly1 = solveEqn(str+lidx, DBHead, &charsRead, ShouldPrintError);
            if (poly1==NULL)
            {
                deleteEqnLkList(Head);
                return NULL;
            }
            shouldBeFreed = 1;
            lidx += charsRead;
            ridx = lidx;
        }
        else
        {
            char temp_for_0; // null character is put at the end and passed to function to get poly
            while(str[ridx]!='*' && str[ridx]!='+' && str[ridx]!='-' && str[ridx]!=' ' && str[ridx]!=0 && str[ridx]!=')') ridx++;
            temp_for_0 = str[ridx];
            str[ridx] = 0;

            // str+lidx is the element after signs, it can be name of poly, or a poly expression
            if (isValidPolyName(str+lidx, 0)) {
                poly1 = getPolyFromDB(DBHead, str+lidx);
            }
            else {
                poly1 = PolyParser(str+lidx, 0);
                // DisplayPoly(poly1);
                shouldBeFreed = 1;
            }

            if (poly1==NULL) {
                // Error
                if (ShouldPrintError) {
                    printf("Error occurred while parsing expression:\n");
                    printf("Unknown element: %s\n", str+lidx);
                }
                deleteEqnLkList(Head);
                return NULL;
            }

            str[ridx] = temp_for_0;
            if (shouldNegatePoly) {
                // if poly1 is newly created(shouldBeFreed==1) we just negate it,
                // if poly1 is referenced to database, we duplicate it and negate it and mark with flag shouldBeFreed;
                if (shouldBeFreed==0) {
                    poly1 = CopyPoly(poly1);
                }
                NegatePoly(poly1);
                shouldBeFreed = 1;
            }

        }
        body->next = NEW_EQN_NODE();
        body = body->next;
        body->sign = sign;
        body->poly1 = poly1;
        body->shouldBeFreed = shouldBeFreed;
        body->next = NULL;

        lidx = ridx;
    }
    body->next = NewDefaultEqnNode();

    // ! For Debugging
    // body = Head;
    // while(body)
    // {
    //     printf("%d\n", body->sign);
    //     displayPrPoly(body->poly1);
    //     printf("%d\n", body->shouldBeFreed);
    //     body = body->next;
    // }

    poly* res = InitializePoly();
    poly* buffpoly1;
    body = Head->next;
    while(body && body->poly1) // poly1 may be NULL sometimes, seg fault if pass to addition function
    {
        if (body->next && body->next->sign == 2) {
            buffpoly1 = MulPolys(body->poly1, body->next->poly1);
            if (body->shouldBeFreed) DeletePoly(body->poly1);
            if (body->next->shouldBeFreed) DeletePoly(body->next->poly1);

            eqnNode* buffEqnNode = body->next->next;
            free(body->next);
            body->next = buffEqnNode;

            body->poly1 = buffpoly1;
            body->shouldBeFreed = 1;
        }
        else {
            AddPolyToPoly(body->poly1, res, body->sign);
            body = body->next;
        }
    }

    // displayPrPoly(res);
    deleteEqnLkList(Head);
    *pCharsRead = ridx;
    return res;
}
