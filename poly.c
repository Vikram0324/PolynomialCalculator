// #pragma once
#include "poly.h"

poly* InitializePoly()
{
    poly* poly1 = NEW_POLY_NODE();
    poly1->power = 0;
    poly1->coeff = 0;
    poly1->next = NULL;
    return poly1;
}

poly* NewIdentityPoly()
{
    // contains a single terms i.e. +1

    poly* poly1Head = InitializePoly();
    poly1Head->next = NEW_POLY_NODE();
    poly* poly1 = poly1Head->next;
    poly1->coeff = 1;
    poly1->power = 0;
    poly1->next = 0;
    return poly1Head;
}

void AddTerm(poly *poly1, int coeff, int power)
{
    // new polynomial case or the case where power > highest power of the polynomial
    if (poly1->next == NULL || power > poly1->next->power)
    {
        poly* temp = poly1->next; // for insertion
        poly1->next = NEW_POLY_NODE();
        poly1 = poly1->next;
        poly1->coeff = coeff;
        poly1->power = power;

        poly1->next = temp;
        return;
    }

    // ignoring head;
    poly1 = poly1->next;
    while(1)
    {
        // if it finds a node with same power, coeff will be added
        if (poly1->power == power)
        {
            poly1->coeff += coeff;
            break;
        }
        // if new power, interts in between
        if (poly1->next==NULL || (poly1->power > power && power > poly1->next->power))
        {
            poly *temp = poly1->next;
            poly1->next = NEW_POLY_NODE();
            poly1 = poly1->next;
            poly1->power = power;
            poly1->coeff = coeff;
            poly1->next = temp;
            break;
        }
        poly1 = poly1->next;
    }
}

void DisplayPoly(poly* poly1)
{
    /* 
        prints 0 if empty poly
        if coeff== 1 or -1: doesn't print that coeff, prints only sign
        if power== 0: doesn't print " x^0 "
        if power== 1: doesn't print " ^1 "
        -
        + sign will not be printed for first term
    */
    if (poly1==NULL) {printf("Null Poly\n"); return;}

    poly1 = poly1->next; // ignoring the head
    if (poly1==NULL) {printf("0\n"); return;}
    // for first term, we don't want to print + sign
    // should check if poly1 is not null, hence using else if
    if (poly1->power != 0)
    {
        if (poly1->coeff==-1) printf("-");
        else if (poly1->coeff != 1) printf("%d", poly1->coeff);
        printf("x");
        if (poly1->power != 1) printf("^%d", poly1->power);
        printf(" ");
    }
    else printf("%d ", poly1->coeff);
    poly1 = poly1->next;
    // printing the following terms with sign
    while(poly1)
    {
        if (poly1->power != 0)
        {
            if (poly1->coeff==-1)  printf("-");
            else if (poly1->coeff== 1)  printf("+");
            else printf("%+d", poly1->coeff);
            printf("x");
            if (poly1->power != 1) printf("^%d", poly1->power);
            printf(" ");
        }
        else printf("%+d ", poly1->coeff);

        poly1 = poly1->next;
    }
    printf("\n");
}

void rmZeroTerms(poly* poly1)
{
    // should also check poly1, otherwise seg fault when poly1=NULL 
    // which is just changed(updated to NULL) in previous iteration
    while(poly1 && poly1->next) 
    {
        if (poly1->next->coeff == 0)
        {
            poly* temp = poly1->next->next;
            free(poly1->next);
            poly1->next = temp;
        }
        else poly1 = poly1->next;
    }
}

void AddPolyToPoly(poly* src, poly* dest, int multiplier) // source and destination
{
    // dest = dest + multiplier*src

    poly* destSaved = dest; // saved this to remove zero terms after the calculation
    
    // Similar to merging sorted linked lists into one of the lists (dest in this function)
    // Similar to add term function
    src = src->next; // ignoring head
    if (src == NULL) return; // if src is empty

    // if dest is new polynomial or if src highest power > dest highest power
    if (dest->next == NULL || src->power > dest->next->power)
    {
        poly* temp = dest->next; // for insertion
        dest->next = NEW_POLY_NODE();
        dest = dest->next;
        dest->coeff = multiplier*(src->coeff);
        dest->power = src->power;
        dest->next = temp; // insertion

        src = src->next; // src used
    }
    else dest = dest->next; // ignoring head

    while(src)
    {
        // similar to Add term process for every src term
        while(1)
        {
            // if it finds the node with same power as current src node, coeff will be added
            if (dest->power == src->power)
            {
                dest->coeff += (multiplier*(src->coeff));
                break;
            }
            // if new power, inserts in between
            if(dest->next==NULL || (dest->power > src->power && src->power > dest->next->power))
            {
                poly* temp = dest->next; // for insertion
                dest->next = NEW_POLY_NODE();
                dest = dest->next;
                dest->coeff = multiplier*(src->coeff);
                dest->power = src->power;

                dest->next = temp;
                break;
            }
            dest = dest->next;
        }
        src = src->next;
    }
    rmZeroTerms(destSaved);
}

poly* AddPolys(poly* poly1, poly* poly2)
{
    poly* poly3 = CopyPoly(poly1);
    AddPolyToPoly(poly2, poly3, 1);
    return poly3;
}

poly* MulPolys(poly* poly1, poly* poly2)
{
    poly* result = InitializePoly();
    poly1 = poly1->next; poly2 = poly2->next; // ignoring heads
    if (poly1 == NULL || poly2 == NULL) return result;
    /* Idea
        take one term of poly1
            create a new IntermediateResult poly
            multiply poly1 term with every term of poly2, keep appending the result to the IntermediateResult poly
                because, when multiplied with the same term, powers may change, but order of powers remains same
            Add the intermediateResult poly to result poly
            free(DeletePoly) intermediateResult poly
    */
    poly* InterResult;
    poly* InterResultHead; // saved to call AddPolyToPoly function later
    poly* poly2t; //temp, for multiplication
    while(poly1)
    {
        InterResult = InitializePoly();
        InterResultHead = InterResult;
        poly2t = poly2;
        while(poly2t)
        { 
            InterResult->next = NEW_POLY_NODE();
            InterResult = InterResult->next;
            InterResult->coeff = poly1->coeff * poly2t->coeff;
            InterResult->power = poly1->power + poly2t->power;
            InterResult->next = NULL;

            poly2t = poly2t->next;
        }
        
        AddPolyToPoly(InterResultHead, result, 1);
        DeletePoly(InterResultHead);

        poly1 = poly1->next;
    }
    return result;
}

void DeletePoly(poly* poly1)
{
    if(poly1){
        DeletePoly(poly1->next);
        free(poly1);
    }
}

poly* CopyPoly(poly* src)
{
    // creates an empty poly and uses AddPolyToPoly function
    poly* dupPoly = InitializePoly();
    AddPolyToPoly(src, dupPoly, 1);
    return dupPoly;
}

void NegatePoly(poly* poly1)
{
    while(poly1->next)
    {
        poly1 = poly1->next;
        poly1->coeff *= -1;
    }
}

poly* PolyParser(char *expr, int PrintError)
{
    poly* poly1 = InitializePoly();
    /* It should understand this way
        can use upper X or lower x
        x   -> term 1x^1
        -x  -> term -1^1
        -23 -> -23x^0
        etc..
    */
    int l=0, r=0; // indices for parsing, string between positions l and r is given to atoi funtion to get integers, [l, r) 
    int coeff, power;
    char temp;
    int first_term = 1;
    int parsingERROR = 0;

    while(expr[r])
    {
        coeff = 1; // set to one by default

        // ignoring leading white spaces
        while(expr[r]==' ') r++;
        if(expr[r]==0 || expr[r]=='\n') break;
        l=r; // l is set to see the starting of co-eff

        // checking for sign
        if (expr[r]=='+') { coeff = 1; r++;}
        else if (expr[r]=='-') { coeff = -1; r++;}
        // if there is no sign and also it is not a first term(which is an exception for sign)
        else if(first_term==0)
        {
            parsingERROR = 1;
            break;
        }

        // ignoring the spaces between sign and polynomial term
        while (expr[r]==' ') r++;

        // if there is sign then there should be digits or x after sign
        // the following if is case of failure, r will be greater than l only if there existed a sign
        if (r>l && !(('0'<=expr[r] && expr[r]<='9') || expr[r]=='x' || expr[r]=='X') )
        {
            parsingERROR = 1;
            break;
        }

        // l is set to starting of the term(after ignoring spaces) to calculate 
        l=r;
        while('0'<=expr[r] && expr[r]<='9')
        {
            r++;
        }

        // calculating coeff, calculating atoi only if digits existed after +sign (condition: r>l). Otherwise, atoi will return 0
        if (r>l)
        {
            temp = expr[r];
            expr[r] = 0; // this is done to call atoi function, will be replaced back with origianl value
            coeff = coeff*atoi(expr+l);
            expr[r] = temp;
        }

        // Constant case
        if (expr[r]==' ' || expr[r]==0 || expr[r]=='+' || expr[r]=='-')
        {
            AddTerm(poly1, coeff, 0);
            l=r; // l is set to see next term
        }

        else if (expr[r] == 'x' || expr[r] == 'X')
        {
            r++; // ignoring x
            if(expr[r] == '^')
            {
                r++; // ignoring ^
                l=r; // l points to first char after ^
                // calculating power. similar to coeff
                if (expr[r]=='+' || expr[r]=='-') r++; // checking for sign

                // if there are no digits after ^
                if (!('0'<=expr[r] && expr[r]<='9'))
                {
                    parsingERROR = 1;
                    break;
                }

                while('0'<=expr[r] && expr[r]<='9')
                {
                    r++;
                }
                // calculating power. Similar to calculation of coeff
                temp = expr[r];
                expr[r] = 0;
                power = atoi(expr+l);
                expr[r] = temp;

                AddTerm(poly1, coeff, power);
                l=r; // l is set to see next term
            }
            // x without any power case (power = 1)
            else if (expr[r]==' ' || expr[r]==0 || expr[r]=='+' || expr[r]=='-')
            {
                AddTerm(poly1, coeff, 1);
                l=r; // l is set to see next term
            }
            else
            {
                parsingERROR = 1;
                break;
            }
        }
        first_term = 0;
    }
    if(parsingERROR)
    {
        // shows the place where error happened with "^"
        // frees the half parsed polynomial,
        // returns NULL
        if (PrintError)
        {
            printf("ERROR: Could not parse the polynomial:\n");
            printf("\t%s\n\t", expr);
            for (int i = 0; i < r; i++) printf(" "); // for empty spaces before ^
            printf("^\n");
        }

        DeletePoly(poly1);
        return NULL;
        
    }
    rmZeroTerms(poly1);
    return poly1;
}
