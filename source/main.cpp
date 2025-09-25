#include "main.h"

// #define EMPTY_ELEM 


int main() {
    
    FILE* fileerr = fopen("fileerr.log", "w");
    
    Stack_t stk1 = {};
    StackCtor(fileerr, &stk1, 5);

    StackPush(fileerr, &stk1, 10);
    StackPush(fileerr, &stk1, 20);
    StackPush(fileerr, &stk1, 3000);

    StackElement_t val = 0;
    StackPop(fileerr, &stk1, &val);
    StackPop(fileerr, &stk1, &val);
    StackPop(fileerr, &stk1, &val);
    StackPop(fileerr, &stk1, &val);

    printf("%d\n", val);

    StackDump(fileerr, &stk1, STACK_OK);

    StackDtor(fileerr, &stk1);
    fclose(fileerr);
}