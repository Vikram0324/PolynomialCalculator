// #pragma once
#include "polyDB.h"

int isValidPolyName(char name[], int PrintError)
{
    int idx=0;
    if (name[0]==0) goto InvalidCase;
    if ((name[0]=='x' || name[0]=='X') && name[1]==0) goto InvalidCase;
    if ('0'<=name[0] && name[0]<='9') goto InvalidCase;
    
    while(name[idx])
    {
        if ( !(('0'<=name[idx] && name[idx]<='9') 
            || ('a'<=name[idx] && name[idx]<='z') 
            || ('A'<=name[idx] && name[idx]<='Z')
            || name[idx]=='_' ) ) 
        {
            goto InvalidCase;
        }
        idx++;
    }
    // if (name[idx-1]=='x' || name[idx-1]=='X') goto InvalidCase;

    return 1;

    InvalidCase: // ------------------ goto label
    if (PrintError)
    {
        printf("   Invalid polynomial name: %s\n", name);
        for (int i = 0; i < 28+idx; i++)  printf(" ");
        printf("^\n");
    }
    return 0;
}

poly* getPolyFromDB(pDBNode* DBHead, const char *polyName)
{
    pDBNode* DBNode = DBHead->next;
    while(DBNode)
    {
        if (strcmp(DBNode->polyName, polyName)==0)
        {
            return DBNode->poly1;
        }
        DBNode = DBNode->next;
    }
    return NULL;
}

int getDBNodeFromDB(pDBNode* DBHead, const char *polyName, pDBNode **DBNodeForReturn)
{
    // transversing through DB, if not found puts the last node in DBNodeForReturn and returns 0
    // if found, puts that node in DBNodeForReturn and returns 1
    if (DBHead->next == NULL)
    {
        *DBNodeForReturn = DBHead;
        return 0;
    }
    pDBNode* DBNode = DBHead->next;
    while(1)
    {
        if (strcmp(DBNode->polyName, polyName)==0)
        {
            *DBNodeForReturn = DBNode;
            return 1;
        }
        if (DBNode->next == NULL)
        {
            *DBNodeForReturn = DBNode;
            return 0;
        }
        DBNode = DBNode->next;
    }
}

void AddNewNodeToDB(pDBNode* DBHead, const char *polyName, poly* poly1)
{
    // The polyName is NOT directly used, it is copied
    // transverse the DBHead, if you find the node with the same name, delete that poly and put this poly in place, if not, append it

    pDBNode* DBNode;
    if (getDBNodeFromDB(DBHead, polyName, &DBNode))
    {
        // Deleting the previous poly
        DeletePoly(DBNode->poly1);
        DBNode->poly1 = poly1;
        return;
    }
    // if not, inserting new node at the end(DBNode will be last node if poly not found)
    
    // Duplicating polyName to store copy of the name in DB
    int polyNameLen = strlen(polyName);
    char *polyNameDup = (char*)malloc(polyNameLen+1);
    strcpy(polyNameDup, polyName);

    DBNode->next = NEW_DB_NODE();
    DBNode = DBNode->next;
    DBNode->polyName = polyNameDup;
    DBNode->poly1 = poly1;
    DBNode->next = NULL;
    return;
}

void listDB(pDBNode* DBHead, char padding)
{
    // lists polynomials in format polyName: polynomial
    // prints "Database is empty" if database is empty
    pDBNode* DBNode = DBHead;
    DBNode = DBNode->next; // ignoring DBHead
    
    if(DBNode == NULL) printf("Database is empty\n");
    while(DBNode)
    {
        if (padding) printf("%12s = ", DBNode->polyName);
        else printf("%s = ", DBNode->polyName);
        DisplayPoly(DBNode->poly1);
        DBNode = DBNode->next;
    }
}

void DeleteDBNode(pDBNode* DBHead, const char *polyName)
{
    // not just freeing this particular node, we have to free everything which is in that node
    pDBNode* DBNode = DBHead;
    pDBNode* target, *temp;
    while(DBNode->next)
    {
        if(strcmp(DBNode->next->polyName, polyName)==0)
        {
            target = DBNode->next;
            // deleting contents inside target DB Node
            free(target->polyName);
            DeletePoly(target->poly1);

            // deleting target DB Node
            temp = target->next;
            free(target);
            DBNode->next = temp;
            return;
        }
        DBNode = DBNode->next;
    }
    printf("ERROR: Unknown polynomial %s\n", polyName);
}

void DeleteDB(pDBNode* DBHead)
{
    pDBNode* node = DBHead->next;
    pDBNode* next;
    while(node)
    {
        next = node->next;
        free(node->polyName);
        DeletePoly(node->poly1);
        node = next;
    }
    DBHead->next = NULL;
}