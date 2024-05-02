#pragma once

#include "poly.h"

// polynomial Data Base Node
// convention: empty head first, data is linked to the head 
struct PolyDBNode
{
    char* polyName;
    poly* poly1;
    struct PolyDBNode* next;
};
typedef struct PolyDBNode pDBNode;
#define NEW_DB_NODE() (pDBNode*)malloc(sizeof(pDBNode));

int isValidPolyName(char name[], int PrintError); // checks if it is a valid polynomial name according to naming rules. if PrintError: prints where the rule is violated

// input: polynomial name -> returns poly
poly* getPolyFromDB(pDBNode* DBHead, const char *polyName);

// transversing through DB, if not found puts the last node in DBNodeForReturn and returns 0
// if found, puts that node in DBNodeForReturn and returns 1
int getDBNodeFromDB(pDBNode* DBHead, const char *polyName, pDBNode **DBNodeForReturn);

// polyName is not used directly, it will be duplicated
void AddNewNodeToDB(pDBNode* DBHead, const char *s, poly* poly1);
void listDB(pDBNode* DBHead, char padding); // prints whole data base in name : poly format
void DeleteDBNode(pDBNode* DBHead, const char *polyName);
void DeleteDB(pDBNode* DBHead);