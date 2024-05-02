#pragma once
#include "poly.h"
#include "polyDB.h"

struct EqnNode // stores polynomial and it's corresponding sign
{
    char sign; // -1 for -, 1 for +, 2 for *
    poly* poly1;
    char shouldBeFreed; // sometimes we have to store intermediate results, these will not be useful once we evaluate our final result, so we should free them
    struct EqnNode* next;
};
typedef struct EqnNode eqnNode;
#define NEW_EQN_NODE() (eqnNode*)malloc(sizeof(eqnNode))

eqnNode* NewDefaultEqnNode();
void deleteEqnLkList(eqnNode* node); // frees eqn linked list, frees the poly in the nodes if shouldBeFreed flag is marked
poly* solveEqn(char *str, pDBNode* DBHead, int* pCharsRead, char ShouldPrintError);
