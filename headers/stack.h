#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #define VERIFY(FILE, Stack_t*, __LINE__) (StackVerify(fileerr, stk, line))

enum StackErr_t {STACK_OK = 0, ERR_SIZE = 1, ERR_ELEM = 2, ERR_STACK_ADRESS = 4, ERR_DATA_ADRESS = 8, ERR_KANAREYKA = 16};


typedef int StackElement_t;

const StackElement_t POISON = 0xDEDDEAD;
const StackElement_t KANAREYKA = 0xDEAD;

typedef struct {
    int line;
    char* filename; 
} Passport_t;

typedef struct {
    StackElement_t* data;
    size_t size;
    size_t capasity;
    // Passport_t passport;
} Stack_t;

StackErr_t StackCtor(FILE* fileerr, Stack_t* stk, size_t capasity);
StackErr_t StackPush(FILE* fileerr, Stack_t* stk, StackElement_t elem);
StackErr_t StackPop(FILE* fileerr, Stack_t* stk, StackElement_t* elem);
StackErr_t StackDtor(FILE* fileerr, Stack_t* stk);
StackErr_t StackDump(FILE* fileerr, Stack_t* stk, StackErr_t err);
StackErr_t StackVerify(FILE* fileerr, Stack_t* stk);

#endif 