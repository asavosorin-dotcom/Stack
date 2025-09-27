#include "stack.h"

//memset

// 0 0 0 0 0 0 0 0

// 

int StackCtor(FILE* fileerr, Stack_t* stk, int capasity, int line, const char* filename) {
    stk->canary = KANAREYKA;
    stk->passport.line = line;
    stk->passport.filename = filename;
    stk->size = 1;
    if (capasity <= 0) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: capasity less than 0\n", __func__, line);
        return ERR_CAPASITY;
    }

    stk->capasity = capasity;

    // fprintf(fileerr, "capasity = %d\n", stk->capasity);
    stk->data = (StackElement_t* ) calloc((size_t) capasity + 2, sizeof(StackElement_t));

    if (stk->data == NULL) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: pointer data is NULL\n", __func__, line);
        return ERR_DATA_ADRESS;
    }

    // STACKVERIFY(stk);

    stk->data[0] = KANAREYKA;

    for (int i = 1; i <= capasity; i++) { // инициализация POISON
        stk->data[i] = POISON;
    }

    stk->data[capasity + 1] = KANAREYKA;

    #ifdef DEBUG
    int err = STACKVERIFY(stk);
    return err;
    #endif

    return 0;
}
// ver
int StackPush(FILE* fileerr, Stack_t* stk, StackElement_t elem, int line) {
    assert(stk);
    int err = 0;

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    if (err) 
        return err;
    #endif
    
    // static int i = 0;

    stk->data[stk->size++] = elem;
    // fprintf(fileerr, "%d", i++);

    #ifdef DEBUG
    err = STACKVERIFY(stk);
    #endif
    return err;
}

int StackPop(FILE* fileerr, Stack_t* stk, StackElement_t* elem, int line) {
    assert(stk);
    assert(elem);

    int err = 0;
    #ifdef DEBUG
    err = STACKVERIFY(stk);
    #endif

    if (stk->size == 1) {
        // printf("Empty stack\n");
        *elem = POISON;
        return STACK_OK;
    }

    *elem = stk->data[--(stk->size)];
    stk->data[stk->size] = POISON;

    #ifdef DEBUG
    err = STACKVERIFY(stk);
    #endif

    return err;
}

int StackTop(FILE* fileerr, Stack_t* stk, StackElement_t* elem, int line) {
    
    int err = 0;
    #ifdef DEBUG
    err = STACKVERIFY(stk);
    #endif
    
    if (err)
        return err;

    if (stk->size == 1) {
        // printf("Empty stack\n");
        *elem = POISON;
        return STACK_OK;
    }
    
    *elem = stk->data[stk->size - 1];

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    #endif

    return err;
}

int StackDtor(FILE* fileerr, Stack_t* stk, int line) {
    
    int err = 0;

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    if (err) 
        return err;
    #endif

    assert(stk);
    stk->size = 0;
    stk->capasity = 0;
    stk->passport = {0};
    free(stk->data);
    stk->data = NULL;

    return err;

}

//0x10 
//0x55

int StackRealloc(FILE* fileerr, Stack_t* stk, int line) {
    int err = 0;

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    if (err) 
        return err;
    #endif

    StackElement_t* stkreal = (StackElement_t* ) realloc(stk->data, sizeof(StackElement_t) * (size_t) stk->capasity * 2 + 2);

    if ((size_t) stkreal != (size_t) stk->data) {
        stk->data = stkreal;
    }
    
    for (int i = stk->capasity + 1; i < stk->capasity * 2 + 1; i++) {
        stk->data[i] = POISON;
    }

    stk->capasity = stk->capasity * 2;
    stk->data[stk->capasity + 1] = KANAREYKA;

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    if (err) 
        return err;
    #endif
    return 0;
}

int StackDump(FILE* fileerr, Stack_t* stk, int err) {
    assert(fileerr);
    assert(stk);

    if (!err) {
        fprintf(fileerr, "Stack OK\n");
        return 0;
    }

    // 1 & 2;
    (err & ERR_CANARY_STRUCT) ? fprintf(fileerr, "Destroyd stack\n") : fprintf(fileerr, "Struct doesn't destroyd\n") ;
    
    fprintf(fileerr, "stack[%p] %s was made in line %d {", stk, stk->passport.filename, stk->passport.line);
    (err & ERR_STACK_ADRESS) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "capasity  = %d ", stk->capasity);
    (err & ERR_CAPASITY) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "data = [%p]", stk->data);
    (err & ERR_DATA_ADRESS) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "size = %zu ", stk->size); // BAD!!!!
    (err & ERR_SIZE) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");



    fprintf(fileerr, "{\n");

    fprintf(fileerr, "![0] = " TYPEELEM " (KANAREYKA) ", stk->data[0]);
    (err & ERR_KANAREYKALEFT) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");
    
    for (int i = 1; i < stk->capasity + 1; i++) {
        if (stk->data[i] == POISON)
            fprintf(fileerr, "[%d] = " TYPEELEM " (POISON)\n", i, stk->data[i]);
        else 
            fprintf(fileerr, "*[%d] = " TYPEELEM "\n", i, stk->data[i]);

    }

    fprintf(fileerr, "![%d] = " TYPEELEM " (KANAREYKA) ", stk->capasity + 1,  stk->data[stk->capasity + 1]);
    (err & ERR_KANAREYKARIGHT) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "}\n}\n\n");

    return err;
}

//0b00
//|
//0b01
//0b01

#ifdef DEBUG

int StackVerify(FILE* fileerr, Stack_t* stk, int line, const char* funcname) {
    int err = 0;

    if (stk->canary != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: change all struct \n", funcname, line);
        return ERR_CANARY_STRUCT;

    }

    if (stk == NULL) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: pointer stk is NULL\n", funcname, line);
        err |= ERR_STACK_ADRESS;
    }
    
    if (stk->capasity <= 0) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: capasity less than 0\n", funcname, line);
        err = err | ERR_CAPASITY;
    }

    if (stk->data == NULL) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: pointer data is NULL\n", funcname, line);
        err = err | ERR_DATA_ADRESS;
    }
    
    if (stk->size == 0 || (int) stk->size == stk->capasity + 1) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: SizeError\n", funcname, line);
        err = err | ERR_SIZE;
    }

    if (stk->data[0] != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: kanareyka left is dead\n", funcname, line);
        err = err | ERR_KANAREYKALEFT;
    }

    if (stk->data[stk->capasity + 1] != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: kanareyka right is dead\n", funcname, line);
        err = err | ERR_KANAREYKARIGHT;
    }
    StackDump(fileerr, stk, err);
    
    return err;
}

#endif

//PUSH, OUT - pechat', ADD, SUB, MUL, DIV, SQRT, HLT - закончить