#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "colors.h"

#define DEBUG

typedef int StackElement_t;
#define TYPEELEM "%d"

enum StackErr_t {STACK_OK = 0, ERR_SIZE = 1, ERR_ELEM = 2, ERR_STACK_ADRESS = 4, ERR_DATA_ADRESS = 8, ERR_KANAREYKALEFT = 16, ERR_KANAREYKARIGHT = 32, ERR_CAPASITY = 64, ERR_CANARY_STRUCT = 128};
    
typedef struct {
    int line;
    const char* filename; 
} Passport_t;
    
typedef struct {
    StackElement_t canary;
    StackElement_t* data;
    size_t size;
    int capasity; 
    Passport_t passport;
    
} Stack_t;

int StackCtor(FILE* fileerr, Stack_t* stk, int capasity, int line, const char* funcname);
int StackPush(FILE* fileerr, Stack_t* stk, StackElement_t elem, int line);
int StackPop(FILE* fileerr, Stack_t* stk, StackElement_t* elem, int line);
int StackDtor(FILE* fileerr, Stack_t* stk, int line);
int StackDump(FILE* fileerr, Stack_t* stk, int err);
int StackTop(FILE* fileerr, Stack_t* stk, StackElement_t* elem, int line);
int StackVerify(FILE* fileerr, Stack_t* stk, int line, const char* funcname);
int StackRealloc(FILE* fileerr, Stack_t* stk, int line);
 
#ifdef DEBUG
#define STACKVERIFY(stk) StackVerify(fileerr, stk, line, __func__);
#endif

#define INITSTACK(stk, capasity) StackCtor(fileerr, &stk, capasity, __LINE__, #stk);

#define PUSH(stk, elem) StackPush(fileerr, &stk, elem, __LINE__);
#define POP(stk, elem) StackPop(fileerr, &stk, &elem, __LINE__);
#define TOP(stk, elem) StackTop(fileerr, &stk, &elem, __LINE__);
#define REALLOC(stk) StackRealloc(fileerr, &stk, __LINE__);
#define DTOR(stk) StackDtor(fileerr, &stk, __LINE__);
#define PRINTELEM(elem) printf(TYPEELEM"\n", elem);

const StackElement_t POISON = 0xDEDDEAD;
const StackElement_t KANAREYKA = 0xDEADBABE;

#endif 