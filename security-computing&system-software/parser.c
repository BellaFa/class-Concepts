//  implements a Recursive Descent Parser and Intermediate Code Generator
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 200
#define MAX_SYMBOL_COUNT 50
#define MAX_REG_COUNT 10
//global

instruction *code;
int cIndex;
symbol *table;
int tIndex;
lexeme *list;
int lIndex;
int level;
int registercounter;


char symbolname[20];
    int symindex;
   
    int varlocreg;
    
    int jpcindex;
    int jmpindex;
    
    int loopindx;
    int  arrayindexreg;


void emit(int opname, int reg, int level, int mvalue);
void addToSymbolTable(int k, char n[], int s, int l, int a, int m);
void mark();
int multipledeclarationcheck(char name[]);
int findsymbol(char name[], int kind);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();

void block();
int vardeclaration();
void proceduredeclaration();
void statement();
void call();
void beginend();
void iffunct();
void dowhile();
void read();
void write();
void condition();
void expression();
void term();
void factor();


instruction *parse(lexeme * list, int printTable, int printCode)
{
    // set up global variables
    code = malloc(sizeof(instruction) * MAX_CODE_LENGTH);
    table = malloc(sizeof(symbol) * MAX_SYMBOL_COUNT);
    cIndex = 0;
    tIndex = 0;
    lIndex = 0;

    // Program
    registercounter = -1;
    emit(7, 0, 0, 0); //jmp
    addToSymbolTable(3, "main", 0, 0, 0, 0);
    level = -1;
    block();
    //check error program closed by period
    emit(11, 0, 0, 0);
    code[0].m = table[0].addr;
    int i;
    for(i=0; i<cIndex; i++)
    {
        if(code[i].opcode == 5)
            code[i].m = table[code[i].m].addr;
    }

    // print off table and code
    if (printTable)
        printsymboltable();
    if (printCode)
        printassemblycode();

    // mark the end of the code
    code[cIndex].opcode = -1;
    return code;
}

// calls all the functions 
void block()
{
   
    // updates the level
    level++;
    int pIndex = tIndex - 1;
    int x = vardeclaration();
    proceduredeclaration();
    // adds to the table
    table[pIndex].addr = cIndex;
    emit(6, 0, 0, x); // x +1 FIX
    statement();
    mark();   
    //decrement level when leaving block
    level--;
}

// if theres a var check for errors regarding var and return mem
int vardeclaration()
{
   
     int memorysize = 3; 
     char symbolname[20];
     int arraysize;
     //if there's a var
    if(list[lIndex].type == varsym)
    {
        do
        {//do atleast once
            lIndex++;// increment and check for ident/assign
            if(list[lIndex].type != identsym && list[lIndex].type != assignsym){
                printparseerror(2);
                exit(0);
                }
            if(multipledeclarationcheck(list[lIndex].name) != -1){
                printparseerror(3);
                exit(0);
            }
            strcpy(symbolname,list[lIndex].name);
            lIndex++;
            // if a left bracket
            if(list[lIndex].type == lbracketsym)
            {
                lIndex++;// increment and check for value
                if(list[lIndex].type != numbersym || list[lIndex].value == 0){
                    printparseerror(4);
                    exit(0);
                }
                arraysize = list[lIndex].value;
                lIndex++;
                // if mult div mod add or sub
                if(list[lIndex].type == multsym || list[lIndex].type == divsym, list[lIndex].type == modsym || list[lIndex].type == addsym || list[lIndex].type == subsym)
                {
                    printparseerror(4);
                    exit(0);
                }
                // right bracket
                else if(list[lIndex].type != rbracketsym){
                    printparseerror(5);
                    exit(0);
                }
                lIndex++;
                // adds to symbol table if none of the prior conditionals are met
                addToSymbolTable(2, symbolname, arraysize, level, memorysize, 0);
                memorysize += arraysize;
            }
            else
            {
                addToSymbolTable(1, symbolname, 0, level, memorysize, 0);
                memorysize++;
            }
            // if there's a comma print error
        }while(list[lIndex].type == commasym);

        if(list[lIndex].type == identsym || list[lIndex].type == assignsym)
            printparseerror(6);
            // if semi colon print value
        else if(list[lIndex].type != semicolonsym)
        {
            printparseerror(7);
            exit(0);
        }
        lIndex++;
        return memorysize;
    }
    
    return memorysize;// if no arrays/procedures returns 3
}

// gets the procedure if at procedure 
void proceduredeclaration()
{
 
    
    char symbolname[20];
    //while a procedure is present
    while(list[lIndex].type == procsym)
    {
        lIndex++; // increment and check for ident or assign 
        if(list[lIndex].type != identsym && list[lIndex].type != assignsym){
            printparseerror(2);
            exit(0);
            }
        else if(multipledeclarationcheck(list[lIndex].name) != -1){
            printparseerror(3);
            exit(0);
            }
        strcpy(symbolname, list[lIndex].name);
        lIndex++;
       // error fro semicolon
        if(list[lIndex].type != semicolonsym){
            printparseerror(8);
            exit(0);
        }
        lIndex++;
        addToSymbolTable(3, symbolname, 0, level, 0, 0);
        block();
        //error for semicolon
        if(list[lIndex].type != semicolonsym)
        {
            printparseerror(7);
            exit(0);
        }

        lIndex++;
        emit(2, 0, 0, 0); //ret
    }
}

// updates statements related to index and symbol
void statement()
{
    
  
    //Assignment 
    if(list[lIndex].type == assignsym || list[lIndex].type == identsym )
    {
      
      strcpy(symbolname,list[lIndex].name);
      lIndex++;
      // if there is left bracket
      if(list[lIndex].type == lbracketsym)
      {
      
          lIndex++;
          symindex = findsymbol(symbolname, 2);
          // checks if symbol name is valid
          if(symindex == -1)
          {
              if(findsymbol(symbolname, 1) != -1){
                  printparseerror(11);
                  exit(0);
                  }
              else if(findsymbol(symbolname, 3) != -1){
                  printparseerror(9);
                  exit(0);
                  }
              else{
                  printparseerror(10);
                  exit(0);
                  }
            }
              // updates register variables
              expression();
             arrayindexreg = registercounter;
              if(list[lIndex].type != rbracketsym){
                  printparseerror(5);
                  exit(0);
                  }
              lIndex++;

              
              if(list[lIndex].type != identsym && list[lIndex].type != assignsym ) {// FIX assignsym
                  printparseerror(13);
                  exit(0);
                  }
              lIndex++;
              expression();
              registercounter++;
              if(registercounter >= 10){
                printparseerror(14);
                exit(0);
                }
              emit(1, registercounter, 0, table[symindex].addr); // lit
              emit(13, arrayindexreg, arrayindexreg, registercounter);// add
              registercounter--;
              emit(4, registercounter, level - table[symindex].level, arrayindexreg);// sto
              registercounter -= 2;
            }
          else{
          // finds the specific symbol
              symindex = findsymbol(symbolname, 1);
              if(symindex == -1)
              {
                 
                  if(findsymbol(symbolname, 2) != -1){
                      printparseerror(12);
                      exit(0);
                      }
                  else if(findsymbol(symbolname, 3) != -1){
                      printparseerror(9);
                      exit(0);
                      }
                  else{
                      printparseerror(10);
                      exit(0);
                      }
              }
                      
                  registercounter++;
                  
                  if(registercounter >= 10){
                      printparseerror(14);
                      exit(0);
                      }

                  emit(1, registercounter, 0, table[symindex].addr);
                  varlocreg = registercounter;
                
                  if(list[lIndex].type != identsym && list[lIndex].type != assignsym){ // FIX assignsym
                      printparseerror(13);
                      exit(0);
                      }
                  lIndex++;
                  expression();
                  emit(4, registercounter, level-table[symindex].level, varlocreg);// sto
                  registercounter -= 2;
               
              
            }
        }
        
        
    
        
        
       if(list[lIndex].type == callsym){
          lIndex++;
         if (list[lIndex].type != identsym && list[lIndex].type != assignsym){
            printparseerror(15);
            exit(0);
            }
          symindex = findsymbol(list[lIndex].name, 3);
         if (symindex == -1){
           if (findsymbol(list[lIndex].name, 1) != -1 || findsymbol(list[lIndex].name, 2) != -1){
                printparseerror(15);
                exit(0);
                }
             else{
                printparseerror(10);
                exit(0);
                }
         }
          // call emit to add to values to table
         emit(5,0, level - table[symindex].level, symindex);
         lIndex++;
       }
       
       
       if(list[lIndex].type == beginsym ){
            do{
                  lIndex++;
                  statement();
            } while (list[lIndex].type == semicolonsym);
            if(list[lIndex].type != endsym){
                  if (list[lIndex].type == identsym || list[lIndex].type == callsym || list[lIndex].type == beginsym || list[lIndex].type == ifsym || list[lIndex].type == dosym || list[lIndex].type == readsym || list[lIndex].type == writesym){
                        printparseerror(16);
                        exit(0);
                    }
                  else{
                    printparseerror(17);
                    exit(0);
                    }
            }
    
        lIndex++;
        }
        
        // if a if function
        if(list[lIndex].type == ifsym){
             
              lIndex++;
              condition();
              jpcindex = cIndex;
              emit(8, registercounter,0, 0);
              registercounter--;
              if (list[lIndex].type != questionsym){
                  printparseerror(18);
                  exit(0);
              }
              lIndex++;
              statement();
              if (list[lIndex].type == colonsym){
                  lIndex++;
                  jmpindex = cIndex;
                  emit(7,0,0,0);
                  code[jpcindex].m = cIndex;
                  statement();
                  code[jpcindex].m = cIndex;
              }
              else{
                  code[jpcindex].m = cIndex;
                  }
        }
        
        
        // if a do while
        if(list[lIndex].type == dosym){
             lIndex++;
             loopindx = cIndex;
             statement();
             if(list[lIndex].type != whilesym){
                  printparseerror(19);
                  exit(0);
             }
             lIndex++;
             condition();
             registercounter++;
             // updates register
             if(registercounter >= 10){
                  printparseerror(14);
                  exit(0);
             }
             emit(1, registercounter, 0, 0);
              emit(18, registercounter - 1, registercounter - 1, registercounter);
             registercounter--;
             emit(8, registercounter, 0, loopindx);
             registercounter--;
        }
       
        //if read
        if(list[lIndex].type == readsym){
                  lIndex++;
            if(list[lIndex].type != identsym && list[lIndex].type != assignsym){
                printparseerror(20);
                exit(0);
                }
            strcpy(symbolname, list[lIndex].name);
            lIndex++;
            // for left bracket
            if(list[lIndex].type == lbracketsym)
            {
                lIndex++;
                symindex = findsymbol(symbolname, 2);
                if(symindex == -1)
                {
                    if(findsymbol(symbolname, 1) != -1){
                        printparseerror(11);
                        exit(0);
                        }
                    else if(findsymbol(symbolname, 3) != -1){
                        printparseerror(9);
                        exit(0);
                        }
                    else{
                        printparseerror(10);
                        exit(0);
                        }
        
                    expression();
                    arrayindexreg = registercounter;
                    if(list[lIndex].type != rbracketsym){
                        printparseerror(5);
                        exit(0);
                        }
                    lIndex++;
                    registercounter++;
                    if (registercounter >= 10){
                        printparseerror(14);
                        exit(0);
                        }
                    emit(10, registercounter, 0, 0);//red
                    registercounter++;
                    if (registercounter >= 10){
                        printparseerror(14);
                        exit(0);
                        }
                    emit(1, registercounter, 0, table[symindex].addr); //lit
                    emit(13, arrayindexreg, arrayindexreg, registercounter); //add
                    registercounter--;
                    emit(4, registercounter, level - table[symindex].level, arrayindexreg);// sto
                    registercounter -= 2;
                }
            }
            else
                {
                    // resposible for lit red and sto
                    symindex = findsymbol(symbolname, 1);
                    if(symindex == -1)
                    {
                        if(findsymbol(symbolname, 2) != -1){
                            printparseerror(12);
                            exit(0);
                            }
                        else if(findsymbol(symbolname, 3) != -1){
                            printparseerror(9);
                            exit(0);
                            }
                        else{
                            printparseerror(10);
                            exit(0);
                            }
                        registercounter++;
                          if (registercounter >= 10){
                            printparseerror(14);
                            exit(0);
                            }
                        emit(1, registercounter, 0, table[symindex].addr);//lit
                        varlocreg = registercounter;
                        registercounter++;
                          if (registercounter >= 10){
                            printparseerror(14);
                            exit(0);
                            }
                        emit(10, registercounter, 0, 0);//red
                        emit(4, registercounter, level - table[symindex].level, varlocreg);// sto
                        registercounter--;
                    }
                    }
        }
      
        
        if(list[lIndex].type == writesym){
           lIndex++;
            expression();
            emit(9, registercounter, 0, 0);
            registercounter--;
        }
    
}

// responsible for equals
void condition()
{
   
    expression();
    // equal
    if(list[lIndex].type == eqlsym)
    {
        lIndex++;
        expression();
        emit(18, registercounter - 1, registercounter - 1, registercounter);
        registercounter--;
    }
    // does not equal
    else if(list[lIndex].type == neqsym)
    {
        lIndex++;
        expression();
        emit(19, registercounter - 1, registercounter - 1, registercounter);
        registercounter--;
    }
    // less than
    else if(list[lIndex].type == lsssym)
    {
        lIndex++;
        expression();
        emit(20, registercounter - 1, registercounter - 1, registercounter);
        registercounter--;
    }
    //less than equal
    else if(list[lIndex].type == leqsym)
    {
        lIndex++;
        expression();
        emit(21, registercounter - 1, registercounter - 1, registercounter);
        registercounter--;
    }
    // greater than
    else if(list[lIndex].type == gtrsym)
    {
        lIndex++;
        expression();
        emit(22, registercounter - 1, registercounter - 1, registercounter);
        registercounter--;
    }
    // greater than or equal
    else if(list[lIndex].type == geqsym)
    {
        lIndex++;
        expression();
        emit(23, registercounter - 1, registercounter - 1, registercounter);
        registercounter--;
    }
    else{
        printparseerror(21);
        exit(0);
        }
} //err 21

// checks errors for operations
void expression()
{
    
    // sub
    if (list[lIndex].type == subsym)
    {
        lIndex++;
        term();
        emit(12, registercounter, 0, registercounter);
        while(list[lIndex].type == addsym || list[lIndex].type == subsym)
        {
        // add
            if (list[lIndex].type == addsym)
            {
                lIndex++;
                term();
                emit(13, registercounter - 1, registercounter - 1, registercounter);
                registercounter--;
            }
        // sub
            else
            {
                lIndex++;
                term();
                emit(14, registercounter - 1, registercounter - 1, registercounter);
                registercounter--;
            }

        }
    }
    else
    {
     
        term();
        // sub
        while(list[lIndex].type == addsym || list[lIndex].type == subsym)
        {
          // add
            if (list[lIndex].type == addsym)
            {
                  lIndex++;
                  term();
                  emit(13, registercounter - 1, registercounter - 1, registercounter);
                  registercounter--;
            }
            else
            {
                lIndex++;
                term();
                emit(14, registercounter - 1, registercounter - 1, registercounter);
                registercounter--;
            }
        }
    }

   if (list[lIndex].type == lparenthesissym || list[lIndex].type == identsym || list[lIndex].type == numbersym){
       printparseerror(22);
       exit(0);
       }
}//err 22

// finds the term for factor
void term()
{
 
    factor();
    // mult div mod
    while(list[lIndex].type == multsym || list[lIndex].type == divsym || list[lIndex].type == modsym){
        // mult
        if (list[lIndex].type == multsym){
            lIndex++;
            factor();
           
            emit(15, registercounter - 1, registercounter - 1, registercounter);
            registercounter--;
        }
        // div
        else if(list[lIndex].type == divsym){
            lIndex++;
            factor();
            emit(16, registercounter - 1, registercounter - 1, registercounter);
            registercounter--;
        }

        else{
            lIndex++;
            factor();
            emit(17, registercounter - 1, registercounter - 1, registercounter);
            registercounter--;
        }
    }
}

// updates indexes for specific characters
void factor() // err 5 9 10 11 12 23 24
{
  
    char symbolname[20];
   int arrayindexreg;
   int symindex;
   int varlocreg;
   // checks for both ident and assign
    if(list[lIndex].type == identsym || list[lIndex].type == assignsym)
    {
        strcpy(symbolname, list[lIndex].name);
        lIndex++;
       // symindex = findsymbol(symbolname, 2);
        if(list[lIndex].type == lbracketsym)
        {
            lIndex++;
            symindex = findsymbol(symbolname, 2);
            if(symindex == -1){
                if(findsymbol(symbolname, 1) != -1){
                  printparseerror(11);
                  exit(0);
                }
                else if(findsymbol(symbolname, 3) != -1){
                  printparseerror(9);
                  exit(0);
                }
              else{
                printparseerror(10);
                exit(0);
                }
            }
                
            expression();
            arrayindexreg = registercounter;
            if(list[lIndex].type != rbracketsym){
                printparseerror(5);
                exit(0);
                }
            lIndex++;
            registercounter++;
              if (registercounter >= 10){
                  printparseerror(14);
                  exit(0);
                  }
            emit(1, registercounter, 0, table[symindex].addr);//lit
            emit(13, arrayindexreg, arrayindexreg, registercounter); // add
            registercounter--;
            emit(3, registercounter, level - table[symindex].level, arrayindexreg); //lod
        }
        else
        {
           
            symindex = findsymbol(symbolname, 1);
            if(symindex == -1)
            {
                if(findsymbol(symbolname, 2) != -1){
                    printparseerror(12);
                    exit(0);
                    }
                else if(findsymbol(symbolname, 3) != -1){
                    printparseerror(9);
                    exit(0);
                    }
                else{
                    printparseerror(10);
                    exit(0);
                    }
            }
            registercounter++;
            if (registercounter >= 10){
                printparseerror(14);
                exit(0);
                }
            emit(1, registercounter, 0, table[symindex].addr);//lit
            varlocreg = registercounter;
            emit(3, registercounter, level - table[symindex].level, varlocreg); //lod
        }
    }
    else if(list[lIndex].type == numbersym)
    {
        registercounter++;
          if (registercounter >= 10){
              printparseerror(14);
              exit(0);
              }
        emit(1, registercounter, 0, list[lIndex].value);
        lIndex++;
    }
    else if(list[lIndex].type == lparenthesissym)
    {
        lIndex++;
        expression();
        if(list[lIndex].type != rparenthesissym){
            printparseerror(23);
            exit(0);
            }
        lIndex++;
    }
    else{
        printparseerror(24);
        exit(0);
        }
}




// ---------------------------GIVEN FUNCTIONS------------------------------------------------------



// initializes and adds code struct
void emit(int opname, int reg, int level, int mvalue)
{
  
    code[cIndex].opcode = opname;
    code[cIndex].r = reg;
    code[cIndex].l = level;
    code[cIndex].m = mvalue;
    cIndex++;
}
// initializes table and adds code to struct
void addToSymbolTable(int k, char n[], int s, int l, int a, int m)
{
    table[tIndex].kind = k;
    strcpy(table[tIndex].name, n);
    table[tIndex].size = s;
    table[tIndex].level = l;
    table[tIndex].addr = a;
    table[tIndex].mark = m;
    tIndex++;
}
// updates the level
void mark()
{
    int i;
    for (i = tIndex - 1; i >= 0; i--)
    {
        if (table[i].mark == 1)
            continue;
        if (table[i].level < level)
            return;
        table[i].mark = 1;
    }
}
// if there is already one don't add
int multipledeclarationcheck(char name[])
{
    int i;
    for (i = 0; i < tIndex; i++)
        if (table[i].mark == 0 && table[i].level == level && strcmp(name, table[i].name) == 0)
            return i;
    return -1;
}
// finds the symbol type updates indx
int findsymbol(char name[], int kind)
{
    int i;
    int max_idx = -1;
    int max_lvl = -1;
    for (i = 0; i < tIndex; i++)
    {
        if (table[i].mark == 0 && table[i].kind == kind && strcmp(name, table[i].name) == 0)
        {
            if (max_idx == -1 || table[i].level > max_lvl)
            {
                max_idx = i;
                max_lvl = table[i].level;
            }
        }
    }
    return max_idx;
}
// stores all the outputs for parser errorors
void printparseerror(int err_code)
{
    switch (err_code)
    {
        case 1:
            printf("Parser Error: Program must be closed by a period\n");
            
            break;
        case 2:
            printf("Parser Error: Symbol names must be identifiers\n");
            
            break;
        case 3:
            printf("Parser Error: Confliciting symbol declarations\n");
           
            break;
        case 4:
            printf("Parser Error: Array sizes must be given as a single, nonzero number\n");
            
            break;
        case 5:
            printf("Parser Error: [ must be followed by ]\n");
            
            break;
        case 6:
            printf("Parser Error: Multiple symbols in var declaration must be separated by commas\n");
          
            break;
        case 7:
            printf("Parser Error: Symbol declarations should close with a semicolon\n");
            
            break;
        case 8:
            printf("Parser Error: Procedure declarations should contain a semicolon before the body of the procedure begins\n");
            
            break;
        case 9:
            printf("Parser Error: Procedures may not be assigned to, read, or used in arithmetic\n");
            
            break;
        case 10:
            printf("Parser Error: Undeclared identifier\n");
           
            break;
        case 11:
            printf("Parser Error: Variables cannot be indexed\n");
            
            break;
        case 12:
            printf("Parserr Error: Arrays must be indexed\n");
           
            break;
        case 13:
            printf("Parser Error: Assignment operator missing\n");
            
            break;
        case 14:
            printf("Parser Error: Register Overflow Error\n");
            
            break;
        case 15:
            printf("Parser Error: call must be followed by a procedure identifier\n");
            
            break;
        case 16:
            printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
            
            break;
        case 17:
            printf("Parser Error: begin must be followed by end\n");
           
            break;
        case 18:
            printf("Parser Error: if must be followed by ?\n");
            
            break;
        case 19:
            printf("Parser Error: do must be followed by while\n");
            
            break;
        case 20:
            printf("Parser Error: read must be followed by a var or array identifier\n");
            
            break;
        case 21:
            printf("Parser Error: Relational operator missing from condition\n");
           
            break;
        case 22:
            printf("Parser Error: Bad arithmetic\n");
           
            break;
        case 23:
            printf("Parser Error: ( must be followed by )\n");
           
            break;
        case 24:
            printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, and variables\n");
            
            break;
        default:
            printf("Implementation Error: unrecognized error code\n");
            
            break;
    }

    free(code);
    free(table);
}

// prints out the symbol table at end of program
void printsymboltable()
{
    int i;
    printf("Symbol Table:\n");
    printf("Kind | Name        | Size | Level | Address | Mark\n");
    printf("---------------------------------------------------\n");
    for (i = 0; i < tIndex; i++)
        printf("%4d | %11s | %5d | %4d | %5d | %5d\n", table[i].kind, table[i].name, table[i].size, table[i].level, table[i].addr, table[i].mark);

    free(table);
    table = NULL;
}

void printassemblycode()
{
    int i;
    printf("Line\tOP Code\tOP Name\tR\tL\tM\n");
    for (i = 0; i < cIndex; i++)//cIndex
    {
        printf("%d\t", i);
        printf("%d\t", code[i].opcode);
        switch (code[i].opcode)
        {
            case 1:
                printf("LIT\t");
                break;
            case 2:
                printf("RET\t");
                break;
            case 3:
                printf("LOD\t");
                break;
            case 4:
                printf("STO\t");
                break;
            case 5:
                printf("CAL\t");
                break;
            case 6:
                printf("INC\t");
                break;
            case 7:
                printf("JMP\t");
                break;
            case 8:
                printf("JPC\t");
                break;
            case 9:
                printf("WRT\t");
                break;
            case 10:
                printf("RED\t");
                break;
            case 11:
                printf("HLT\t");
                break;
            case 12:
                printf("NEG\t");
                break;
            case 13:
                printf("ADD\t");
                break;
            case 14:
                printf("SUB\t");
                break;
            case 15:
                printf("MUL\t");
                break;
            case 16:
                printf("DIV\t");
                break;
            case 17:
                printf("MOD\t");
                break;
            case 18:
                printf("EQL\t");
                break;
            case 19:
                printf("NEQ\t");
                break;
            case 20:
                printf("LSS\t");
                break;
            case 21:
                printf("LEQ\t");
                break;
            case 22:
                printf("GTR\t");
                break;
            case 23:
                printf("GEQ\t");
                break;
            default:
                printf("err\t");
                break;
        }
        printf("%d\t%d\t%d\n", code[i].r, code[i].l, code[i].m);
    }

    if (table != NULL)
        free(table);
}
