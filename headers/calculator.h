#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "stack.h"

int add(FILE* fileerr, Stack_t* stk);
int sub(FILE* fileerr, Stack_t* stk);
int mul(FILE* fileerr, Stack_t* stk);
int div(FILE* fileerr, Stack_t* stk);
int sqr(FILE* fileerr, Stack_t* stk);

void Calculate(FILE* fileerr, Stack_t* stk1);

#define ADD(stk) add(fileerr, &stk)
#define SUB(stk) sub(fileerr, &stk)
#define MUL(stk) mul(fileerr, &stk)
#define DIV(stk) div(fileerr, &stk)
#define SQRT(stk) sqr(fileerr, &stk)
#define CALCULATE(stk) Calculate(fileerr, &stk) 

#endif