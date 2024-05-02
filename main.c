#include "poly.h"
#include "polyDB.h"
#include "eqn.h"
// #include "poly.c"
// #include "polyDB.c"
// #include "eqn.c"

// ------------------------ Main -----------------------
int main()
{
    // Initializing Polynomial Data Base
    pDBNode* DBHead = NEW_DB_NODE();
    DBHead->poly1 = NULL;
    DBHead->polyName = NULL;
    DBHead->next = NULL;

    // buffer storage ---------------
    poly* buffPoly1;
    poly* buffPoly2;

    pDBNode* buffDBNode1;
    pDBNode* buffDBNode2;
    char inputbuff1[300];
    char *polyName;
    char *polyName2;
    char *polyName3;
    char *polyExpr;
    char buffchar1;
    // buffer storage -end -----------

    // to tokenize the command string
    const char delimiters[] = " ";
    const int CommandLineLen = 300;
    char CommandLine[300];
    CommandLine[0]=0; CommandLine[CommandLineLen-1]=0;
    int CommandLineIdx = 0;
    char* command = NULL;
    char EndedWithSemiColon = 0;
    // infinite loop
    while(1)
    {
        while (CommandLine[CommandLineIdx]==0) {
            gets_s(CommandLine, 299);
            CommandLineIdx = 0;
        }
        EndedWithSemiColon = 0;
        command = CommandLine+CommandLineIdx;
        while(CommandLine[CommandLineIdx]!=';' && CommandLine[CommandLineIdx]!=0) {
            CommandLineIdx++;
        }
        if (CommandLine[CommandLineIdx]==';') {
            EndedWithSemiColon = 1;
            CommandLine[CommandLineIdx]=0;
            CommandLineIdx++;
        }

        while(*command==' ') command++; // ignoring the leading spaces. Command is not character array, it is char*, so no problem
        if(*command == 0) continue;

        char EqualSymInCommand = 0; // 1 if Equal symbol is in command
        int EqualSymIdx; // Position of equal symbol if it exists
        for (int i = 0; command[i]!=0; i++)
        {
            if (command[i]=='=') {
                EqualSymInCommand = 1;
                EqualSymIdx = i;
                break;
            }
        }

        if (EqualSymInCommand)
        {
            command[EqualSymIdx] = 0;
            char constAssignment = 0;
            if(EqualSymIdx>0 && command[EqualSymIdx-1]==':') {
                constAssignment = 1;
                command[EqualSymIdx-1] = ' '; // space will be removed by the next code snippet
            }

            // removing tailing spaces. spaces before = sign or := sign
            for (int i = EqualSymIdx-1; i>=0 && command[i]==' '; i--) {
                command[i]=0;
            }

            polyName = command;
            if (isValidPolyName(polyName, 1)==0) continue;
            
            if (constAssignment) {
                // printf("here\n");
                buffPoly1 = PolyParser(command+EqualSymIdx+1, 1);
            }
            else {
                int temp;
                buffPoly1 = solveEqn(command+EqualSymIdx+1, DBHead, &temp, 1);
            }
            if (buffPoly1 == NULL) continue;
            AddNewNodeToDB(DBHead, polyName, buffPoly1);
            if ( !EndedWithSemiColon) { // Display the poly if the current command didn't end with semicolon
                int a = printf("%s = \n", polyName);
                for(int i=0; i<a; i++) printf(" ");
                DisplayPoly(buffPoly1);
            }
        }
        else
        {
            if (command[0]!='>' && command[0]!='#' && command[0]!='^' && command[0]!='$' && command[0]!='!' && command[0]!='@' && command[0]!='*')
            {
                int temp;
                // if the command is an expression, buffPoly will be a valid polynomial, then it is stored with ans
                buffPoly1 = solveEqn(command, DBHead, &temp, 1);
                if (buffPoly1) {
                    AddNewNodeToDB(DBHead, "ans", buffPoly1);
                    int a = printf("ans = \n");
                    printf("       ");
                    DisplayPoly(buffPoly1);
                }
                continue;
            }
            // tokenizing the command string
            char *token = strtok(command, delimiters);
            // read poly input command
            if (strcmp(token, ">") == 0) 
            {
                polyName = strtok(NULL, delimiters);

                // from here tokens are names of the polynomials that we are going to take input
                while(polyName != NULL)
                {
                    if(isValidPolyName(polyName, 1)==0) {
                        polyName = strtok(NULL, delimiters);
                        continue;
                    }
                    do {
                        printf("Enter the polynomial %s: ", polyName);
                        scanf(" %[^\n]s", inputbuff1);
                        buffPoly1 = PolyParser(inputbuff1, 1);
                    }while(buffPoly1 == NULL);

                    printf("Entered polynomial: %s = ", polyName); DisplayPoly(buffPoly1);

                    AddNewNodeToDB(DBHead, polyName, buffPoly1);
                    polyName = strtok(NULL, delimiters);
                }
            }
            // display poly command
            else if (strcmp(token, "#") == 0) 
            {
                polyName = strtok(NULL, delimiters);
                // from here tokens are names of the polynomials that we are going to take input
                while(polyName != NULL)
                {
                    printf("%12s = ", polyName);
                    buffPoly1 = getPolyFromDB(DBHead, polyName);
                    if (buffPoly1)
                    {
                        DisplayPoly(buffPoly1);
                    }
                    else
                    {
                        printf("ERROR: Unknown Polynomial\n");
                    }

                    polyName = strtok(NULL, delimiters);
                }
            }
            // rename command
            else if(strcmp(token, "^")==0)
            {
                polyName = strtok(NULL, delimiters);
                if (polyName==NULL) {printf("ERROR: INVALID\n"); continue;} // safety check
                if (getDBNodeFromDB(DBHead, polyName, &buffDBNode1))
                {
                    polyName2 = strtok(NULL, delimiters);
                    if (polyName2==NULL) {printf("ERROR: INVALID\n"); continue;} // safety check
                    if (isValidPolyName(polyName2, 1)==0) continue; // if invalid polyname

                    // if polyName2 already exists in Database, then throws error
                    if (getDBNodeFromDB(DBHead, polyName2, &buffDBNode2))
                    {
                        printf("Polynomial %s already exists in database\n", polyName2);
                        continue;
                    }

                    // overwriting previous name
                    polyName3 = (char*)malloc(strlen(polyName2)+1);
                    strcpy(polyName3, polyName2);
                    free(buffDBNode1->polyName);
                    buffDBNode1->polyName = polyName3;

                    if ( !EndedWithSemiColon) {
                        int temp = printf("%s = \n", buffDBNode1->polyName);
                        while(temp--) printf(" ");
                        DisplayPoly(buffDBNode1->poly1);
                    }
                }
                else {
                    printf("ERROR: Unknown Polynomial %s\n", polyName);    
                }
            }
            // Delete command
            else if(strcmp(token, "!")==0)
            {
                polyName = strtok(NULL, delimiters);
                while(polyName != NULL)
                {
                    DeleteDBNode(DBHead, polyName);
                    polyName = strtok(NULL, delimiters);
                }
            }
            // Delete DataBase command
            else if(strcmp(token, "!!")==0)
            {
                printf("Delete entire database?[Y/N]: ");
                buffchar1 = getchar();
                // printf("%c: %d\n", buffchar1, buffchar1);
                
                if (buffchar1=='y' || buffchar1=='Y') {
                    DeleteDB(DBHead);
                    printf("Cleared entire database\n");
                }
                else {
                    printf("Deletion Cancelled. Database remains unchanged\n");
                }
            }
            // list Data Base command
            else if(strcmp(token, "##")==0 || strcmp(token, "$")==0)
            {
                listDB(DBHead, 1);
            }
            // reading from a file
            else if(strcmp(token, "@r")==0)
            {
                printf("Polynomials read from the file: ");
                char* filename = strtok(NULL, delimiters);
                FILE* file = fopen(filename, "r");
                if (file == NULL) {
                    printf("Error: Couldn't open file: %s\n", filename);
                    continue;
                }
                // if valid line, it will be read, otherwise it will be ignored
                while(fgets(inputbuff1, 299, file)!=NULL)
                {
                    // printf("%s\n", inputbuff1);
                    char* buff = inputbuff1;
                    while(*buff==' ') buff++;

                    polyName = buff;

                    int buffIdx=0;
                    while(buff[buffIdx]!=0 && buff[buffIdx]!=':' && buff[buffIdx]!=' '){
                        buffIdx++;
                    }

                    if (buff[buffIdx]!=':') {
                        if (buff[buffIdx]==' ') {
                            buff[buffIdx] = 0;
                            buffIdx++;
                        }
                        while(buff[buffIdx]==' ') buffIdx++;

                        if (buff[buffIdx]!=':') continue;
                        buff[buffIdx] = 0;
                        buffIdx++;
                        if (buff[buffIdx]!='=') continue;
                    }
                    int temp;
                    buffPoly1 = PolyParser(buff+buffIdx+1, 0);
                    if (buffPoly1==NULL) continue;
                    AddNewNodeToDB(DBHead, polyName, buffPoly1);
                    printf("%s ", polyName);
                }
                fclose(file);
                printf("\n");
            }
            // writing to a file
            else if(strcmp(token, "@w")==0)
            {
                char* filename = strtok(NULL, delimiters);
                FILE* file = freopen(filename, "w", stdout);
                if (file == NULL) {
                    printf("Error: Couldn't open file: %s\n", filename);
                    continue;
                }
                pDBNode* DBnode = DBHead->next;
                if (DBnode==NULL) printf("NULL");
                while(DBnode) {
                    printf("%12s := ", DBnode->polyName);
                    DisplayPoly(DBnode->poly1);
                    DBnode = DBnode->next;
                }
                fclose(file);
                freopen("CON", "w", stdout);
                printf("Polynomials are successfully written to the file\n");
            }
            // exit command
            else if (strcmp(token, "*")==0) 
            {
                printf("The program has terminated\n");
                break;
            }
            else
            {
                printf("ERROR: Unrecognized Command: %s\n", token);
            }
        }
    }
    return 0;
}
