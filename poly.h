#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEW_POLY_NODE() (poly *) malloc(sizeof(poly))

struct Poly
{
    int power;
    int coeff;
    struct Poly* next;
};
typedef struct Poly poly;
// Polynomial starts with a head node(will not be used to store any term of poly)
// the terms of polynomial will be linked to this head node in descending order of powers

// --------------------- function definitions ---------------------
poly* InitializePoly(); // Used to create head of poly
poly* NewIdentityPoly(); // contains a single terms i.e. +1
void AddTerm(poly *poly1, int coeff, int power);
void DisplayPoly(poly* poly1);
void rmZeroTerms(poly* poly1); // removes the terms in poly1 which have zero as co-eff
void AddPolyToPoly(poly* src, poly* dest, int multiplier); // dest = dest + multiplier*src
poly* AddPolys(poly* poly1, poly* poly2);
poly* MulPolys(poly* poly1, poly* poly2);
void DeletePoly(poly* poly1); // frees memory of whole poly linked list
poly* PolyParser(char *expr, int PrintError); // parses the string, creates a new polynomial, returns that polynomial, if PrintError: prints the position of parsing error
poly* CopyPoly(poly* src); // creates a new duplicate given src poly and returns the duplicate
void NegatePoly(poly* poly1); // multiplies all coeffs by -1