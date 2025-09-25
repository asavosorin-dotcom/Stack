#include "stack.h"

//memset

// 0 0 0 0 0 0 0 0

StackErr_t StackCtor(FILE* fileerr, Stack_t* stk, size_t capasity) {
    stk->data = (StackElement_t* ) calloc(capasity + 2, sizeof(StackElement_t));
    stk->capasity = capasity;

    stk->data[0] = KANAREYKA;

    for (size_t i = 1; i <= capasity; i++) { // инициализация POISON
        stk->data[i] = POISON;
    }

    stk->data[capasity + 1] = KANAREYKA;

    StackErr_t err = StackVerify(fileerr, stk);
    return err;
}
// ver
StackErr_t StackPush(FILE* fileerr, Stack_t* stk, StackElement_t elem) {
    assert(stk);
    
    stk->data[1 + stk->size++] = elem;

    StackErr_t err = StackVerify(fileerr, stk);
    return err;
}

StackErr_t StackPop(FILE* fileerr, Stack_t* stk, StackElement_t* elem) {
    assert(stk);
    assert(elem);

    *elem = stk->data[ 1 + --(stk->size)];

    StackErr_t err = StackVerify(fileerr, stk);
    return err;
}

StackErr_t StackDtor(FILE* fileerr, Stack_t* stk) {
    // StackErr_t err = StackVerify(fileerr, stk);
    // return err;
    
    assert(stk);
    
    
    StackErr_t err = StackVerify(fileerr, stk);
    free(stk->data);
    return err;

}

StackErr_t StackDump(FILE* fileerr, Stack_t* stk, StackErr_t err) {
    assert(fileerr);
    assert(stk);
    fprintf(fileerr, "called from line");
    // 1 & 2;
    fprintf(fileerr, "stack[%p] {\n", stk);
    // () ? printf
    fprintf(fileerr, "size = %zu\n", stk->size); // BAD!!!!
    // () ? printf
    fprintf(fileerr, "capasity  = %zu\n", stk->capasity);
    fprintf(fileerr, "data = [%p]", stk->data);
    fprintf(fileerr, "{\n");

    fprintf(fileerr, "[0] = %d (KANAREYKA)\n", stk->data[0]);

    for (size_t i = 1; i < stk->capasity + 1; i++) {
        if (stk->data[i] == POISON)
            fprintf(fileerr, "[%zu] = %d (POISON)\n", i, stk->data[i]);
        else 
            fprintf(fileerr, "*[%zu] = %d\n", i, stk->data[i]);

    }

    fprintf(fileerr, "[%zu] = %d (KANAREYKA)\n", stk->capasity + 1,  stk->data[0]);

    fprintf(fileerr, "}\n}\n\n");

    return STACK_OK;
}

StackErr_t StackVerify(FILE* fileerr, Stack_t* stk) {
    if (stk == NULL) {
        fprintf(fileerr, "ERROR: pointer stk is NULL\n");
        StackDump(fileerr, stk, ERR_STACK_ADRESS);
        return ERR_STACK_ADRESS;
    }

    if (stk->data == NULL) {
        fprintf(fileerr, "ERROR: pointer data is NULL\n");
        StackDump(fileerr, stk, ERR_DATA_ADRESS);
        return ERR_DATA_ADRESS;
    };

    if (stk->capasity <= 0) {
        fprintf(fileerr, "ERROR: capasity less than 0\n");
        StackDump(fileerr, stk, ERR_DATA_ADRESS);
        return ERR_SIZE;
    }

    if (stk->data[0] != KANAREYKA || stk->size == 0) {
        fprintf(fileerr, "ERROR: kanareyka is dead\n");
        StackDump(fileerr, stk, ERR_KANAREYKA);
        return ERR_KANAREYKA;
    }

    return STACK_OK;
}