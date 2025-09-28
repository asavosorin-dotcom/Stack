#include "stack.h"

//memset

// 0 0 0 0 0 0 0 0

// 

int StackCtor(FILE* fileerr, Stack_t* stk, int capasity, int line, const char* filename) {
    stk->canary_left = KANAREYKA;
    stk->canary_right = KANAREYKA;

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

    stk->hash = (StackElement_t) stk->size + (StackElement_t) stk->capasity;

    #ifdef DEBUG
    int err = STACKVERIFY(stk);

    if (!err)
        fprintf(fileerr, "INIT OK\n");

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
    stk->hash += (StackElement_t) stk->size * elem;
    stk->hash += 1; // size += 1

    // fprintf(fileerr, "delta hash = " TYPEELEM "\n", (StackElement_t) stk->size * elem);

    stk->data[stk->size++] = elem;
    // fprintf(fileerr, "%d", i++);

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    if (!err)
        fprintf(fileerr, "PUSH OK\n");

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
        *elem = 0;
        return STACK_OK;
    }

    *elem = stk->data[--(stk->size)];
    stk->hash -= (StackElement_t) stk->size * *elem;
    stk->hash -= 1; // size += 1

    stk->data[stk->size] = POISON;

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    if (!err)
        fprintf(fileerr, "POP OK\n");

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

    if (!err)
        fprintf(fileerr, "TOP OK\n");

    #endif

    return err;
}

int StackDtor(FILE* fileerr, Stack_t* stk, int line) {
    
    int err = 0;

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    if (err == ERR_DATA_ADRESS) 
        return err;
    #endif

    assert(stk);
    stk->size = 0;
    stk->capasity = 0;
    stk->passport = {0};
    free(stk->data);
    // printf("FREEEE!\n");
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

    StackElement_t* stkreal = (StackElement_t* ) realloc(stk->data, (2 * (size_t) stk->capasity + 2) * sizeof(stk->data[0]));
    fprintf(fileerr, "12322314\n");

    if (stkreal != stk->data && stkreal != NULL) {
        stk->data = stkreal;
    }
    
    for (int i = stk->capasity + 1; i < stk->capasity * 2 + 1; i++) {
        fprintf(fileerr, "i = %d\n", i);

        stk->data[i] = POISON;
    }

    stk->capasity = stk->capasity * 2;
    stk->data[stk->capasity + 1] = KANAREYKA;

    #ifdef DEBUG
    err = STACKVERIFY(stk);

    if (err) 
        return err;
    else 
        fprintf(fileerr, "REALLOC OK\n");

    #endif
    return 0;
}

int StackDump(FILE* fileerr, Stack_t* stk, int err) {
    assert(fileerr);
    assert(stk);

    if (!err) {
        return 0;
    }

    
    // 1 & 2;
    (err & ERR_CANARY_STRUCT) ? fprintf(fileerr, "Destroyd stack\n") : fprintf(fileerr, "Struct doesn't destroyd\n") ;

    (err & ERR_HASH) ? fprintf(fileerr, "ERROR HASH") : fprintf(fileerr, "HASH OK");
    
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

    if (stk->canary_left != KANAREYKA || stk->canary_right != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: change struct \n", funcname, line);
        return ERR_CANARY_STRUCT;
    }

    #ifdef HASH

    if (stk->hash != StackHash(stk)) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: change struct \n", funcname, line);
        err |= ERR_HASH;
        // return ERR_HASH;
    }

    #endif

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

#ifdef HASH

StackElement_t StackHash(Stack_t* stk) {
    StackElement_t hash1 = 0;
    hash1 += (StackElement_t) stk->capasity;
    hash1 += (StackElement_t) stk->size;

    for (size_t i = 1; i < stk->size; i++) {
        hash1 += stk->data[i] * (StackElement_t) i;
    } 

    for (int i = (int) stk->size; i < stk->capasity + 1; i++) {
        hash1 += (stk->data[i] == POISON) ? 0: stk->data[i];
    }

    return hash1;
}

#endif

int StackPrint(FILE* fileerr, Stack_t* stk, int line) {
    int err = 0;
    
    #ifdef DEBUG
    err = STACKVERIFY(stk);
    #endif

    for (size_t i = 1; i < stk->size; i++) {
        printf(TYPEELEM " ", stk->data[i]);
    }

    printf("\n");

    return err;
}

//PUSH, OUT - pechat', ADD, SUB, MUL, DIV, SQRT, HLT - закончить