#include "stack.h"

//memset

// 0 0 0 0 0 0 0 0

// fileerr - глобал, везде убрать

int StackCtor(FILE* fileerr, Stack_t* stk, int capasity, int line, const char* stackname, const char* funcname) {
    assert(stk);
    
    stk->canary_left = KANAREYKA; // условная комп
    stk->canary_right = KANAREYKA;

    #ifdef DEBUG
    stk->passport.line = line;
    stk->passport.filename = stackname;
    stk->passport.funcname = funcname;
    #endif

    stk->size = 0;

    ONDEBAGCANARY(stk->size = 1);

    #ifdef DEBUG
    if (capasity <= 0) { // capacity
        fprintf(fileerr, "Called from func: %s line: %d ERROR: capasity less than 0\n", __func__, line);
        return ERR_CAPASITY;
    }
    #endif

    stk->capasity = capasity;

    // fprintf(fileerr, "capasity = %d\n", stk->capasity);
    #ifdef CANARY
        stk->data = (StackElement_t* ) calloc((size_t) capasity + 2, sizeof(StackElement_t));
    #else
        stk->data = (StackElement_t* ) calloc((size_t) capasity, sizeof(StackElement_t));
    #endif

    #ifdef DEBUG
    if (stk->data == NULL) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: pointer data is NULL\n", __func__, line);
        return ERR_DATA_ADRESS;
    }
    #endif

    // STACKVERIFY(stk);

    // много видов условных компилляций

    ONDEBAGCANARY(stk->data[0] = KANAREYKA);

    #ifdef DEBUG 
    int flag = 0;
    ONDEBAGCANARY(flag = 1);

    for (int i = flag; i < capasity + flag; i++) { // инициализация POISON
        stk->data[i] = POISON;
    }
    #else
    for (int i = flag; i <= capasity + flag; i++) { // инициализация POISON
        stk->data[i] = 0;
    }
    #endif

    ONDEBAGCANARY(stk->data[capasity + 1] = KANAREYKA);

    ONDEBAGHASH(stk->hash = ((StackElement_t) stk->capasity + (StackElement_t) stk->size));

    int err = 0;
    ERRPRINTOK("INIT OK\n")

    return 0;
}
// ver
int StackPush(FILE* fileerr, Stack_t* stk, StackElement_t elem, int line) {
    assert(stk);
    int err = 0;

    ERRPRINTOK("")

    // fprintf(fileerr, "fnvfnpwdfnvwp\n");

    // fprintf(fileerr, "size in push = %d\n", (int) stk->size);

    if ((int) stk->size == stk->capasity) {
        // fprintf(fileerr, "Check realloc\n");
        REALLOC(*stk)
    }

    // fprintf(fileerr, "Check push after realloc\n");

    // fprintf(fileerr, "\nhash befor push = %d\n", stk->hash);
    
    #ifdef HASH
    stk->hash += (StackElement_t) stk->size * elem;
    stk->hash += 1; // size += 1
    #endif
    
    stk->data[stk->size++] = elem;

    // fprintf(fileerr, "\nhash after push = %d\n", stk->hash);

    // fprintf(fileerr, "delta hash = " TYPEELEM "\n", (StackElement_t) stk->size * elem);

    // fprintf(fileerr, "%d", i++);

    ERRPRINTOK("PUSH OK\n")

    return err;
}

int StackPop(FILE* fileerr, Stack_t* stk, StackElement_t* elem, int line) {
    assert(stk);
    assert(elem);

    int err = 0;
    
    ERRPRINTOK("")

    #ifdef CANARY

    if (stk->size == 1) {
        // printf("Empty stack\n");
        *elem = 0;
        return STACK_EMPTY; 
    }

    #else 

    if (stk->size == 1) {
        // printf("Empty stack\n");
        *elem = 0;
        return STACK_EMPTY; // STACK_EMPTY
    }

    #endif

    *elem = stk->data[--(stk->size)];

    #ifdef HASH

    stk->hash -= (StackElement_t) stk->size * *elem;
    stk->hash -= 1; // size += 1

    #endif

    #ifdef DEBUG
        stk->data[stk->size] = POISON;
    #else 
        stk->data[stk->size] = 0;
    #endif

    ERRPRINTOK("POP OK\n")

    return err;
}

int StackTop(FILE* fileerr, Stack_t* stk, StackElement_t* elem, int line) {
    
    int err = 0;
    
    ERRPRINTOK("")

   #ifdef CANARY

    if (stk->size == 1) {
        // printf("Empty stack\n");
        *elem = 0;
        return STACK_EMPTY; 
    }

    #else 

    if (stk->size == 0) {
        // printf("Empty stack\n");
        *elem = 0;
        return STACK_EMPTY; // STACK_EMPTY
    }

    #endif
    
    *elem = stk->data[stk->size - 1];

    ERRPRINTOK("TOP OK\n")

    return err;
}

int StackDtor(FILE* fileerr, Stack_t* stk, int line) {
    
    int err = 0;

    ERRPRINTOK("")

    assert(stk);

    stk->size = 0;
    stk->capasity = 0;
    
    ONDEBAG(stk->passport = {0});

    free(stk->data);
    // printf("i WANT TO break; FREEEE!\n");
    stk->data = NULL;

    return err;

}

//0x10 
//0x55

int StackRealloc(FILE* fileerr, Stack_t* stk, int line) {
    int err = 0;

    ERRPRINTOK("")

    #ifdef CANARY
    StackElement_t* stkreal = (StackElement_t* ) realloc(stk->data, (2 * (size_t) stk->capasity + 2) * sizeof(stk->data[0]));
    #else
    StackElement_t* stkreal = (StackElement_t* ) realloc(stk->data, (2 * (size_t) stk->capasity) * sizeof(stk->data[0]));
    #endif

    // fprintf(fileerr, "stkreal[%p]\n", stkreal);

    if (stkreal != NULL) {
        stk->data = stkreal;
    }
    
    #ifdef DEBUG
        int flag = 0;
        ONDEBAGCANARY(flag = 1);
        for (int i = stk->capasity + flag; i < stk->capasity * 2 + flag; i++) {
            // fprintf(fileerr, "i = %d\n", i);

            stk->data[i] = POISON;
        }
    #else
            for (int i = stk->capasity + 1; i < stk->capasity * 2; i++) {
            fprintf(fileerr, "i = %d\n", i);

            stk->data[i] = 0;
        }
    #endif


    ONDEBAGHASH(stk->hash += stk->capasity);

    stk->capasity = stk->capasity * 2;
    // fprintf(fileerr,"capasity = %d from realloc\n ", stk->capasity);
    
    ONDEBAGCANARY(stk->data[stk->capasity + 1] = KANAREYKA);

    ERRPRINTOK("REALLOC OK\n")

    return 0;
}

#ifdef DEBUG

int StackDump(FILE* fileerr, Stack_t* stk, int err) {
    assert(fileerr);
    assert(stk);

    if (!err) {
        return 0;
    }

    
    // 1 & 2;
    (err & ERR_CANARY_STRUCT) ? fprintf(fileerr, "Destroyd stack\n") : fprintf(fileerr, "Struct doesn't destroyd\n") ;

    ONDEBAGHASH((err & ERR_HASH) ? fprintf(fileerr, "ERROR HASH\n") : fprintf(fileerr, "HASH OK\n"));
    
    fprintf(fileerr, "stack[%p] %s was made in line %d function: %s{", stk, stk->passport.filename, stk->passport.line, stk->passport.funcname);
    (err & ERR_STACK_ADRESS) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "capasity  = %d ", stk->capasity);
    (err & ERR_CAPASITY) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "data = [%p]", stk->data);
    (err & ERR_DATA_ADRESS) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "size = %zu ", stk->size); // BAD!!!!
    (err & ERR_SIZE) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    ONDEBAGHASH((fileerr, "hash = " TYPEELEM, stk->hash));


    fprintf(fileerr, "{\n");

    ONDEBAGCANARY(fprintf(fileerr, "![0] = " TYPEELEM " (KANAREYKA) ", stk->data[0]));
    (err & ERR_KANAREYKALEFT) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");
    
    int flag = 0;
    ONDEBAGCANARY(flag = 1);

    for (int i = flag; i < stk->capasity + flag; i++) {
        if (stk->data[i] == POISON)
            fprintf(fileerr, " [%d] = " TYPEELEM " (POISON)\n", i, stk->data[i]);
        else 
            fprintf(fileerr, "*[%d] = " TYPEELEM "\n", i, stk->data[i]);

    }

    ONDEBAGCANARY(fprintf(fileerr, "![%d] = " TYPEELEM " (KANAREYKA) ", stk->capasity + 1,  stk->data[stk->capasity + 1]));
    (err & ERR_KANAREYKARIGHT) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "}\n}\n\n");

    return err;
}

//0b00
//|
//0b01
//0b01

int StackVerify(FILE* fileerr, Stack_t* stk, int line, const char* funcname) {
    int err = 0;    
    // fprintf(fileerr, "verify funcname = %s \n", funcname);

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
        fprintf(fileerr, "Called from func: %s line: %d ERROR: pointer stk is NULL\n", funcname, line); // called from.
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
    // fprintf(fileerr, "size = %d", stk->size);
    if ((int) stk->size == stk->capasity + 1) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: SizeError\n", funcname, line);
        err = err | ERR_SIZE;
    }

    #ifdef CANARY

    if (stk->data[0] != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: kanareyka left is dead\n", funcname, line);
        err = err | ERR_KANAREYKALEFT;
    }

    if (stk->data[stk->capasity + 1] != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: kanareyka right is dead\n", funcname, line);
        err = err | ERR_KANAREYKARIGHT;
    }

    #endif

        // fprintf(fileerr, "funcname = %s, err = %d\n", funcname, err);
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

    // printf("hash1 = " TYPEELEM"\n", hash1);
    return hash1;
}

#endif

int StackPrint(FILE* fileerr, Stack_t* stk, int line) {
    int err = 0;
    
    #ifdef DEBUG
    err = STACKVERIFY(stk);
    #endif

    #ifdef CANARY
    for (size_t i = 1; i < stk->size; i++) {
        printf(TYPEELEM " ", stk->data[i]);
    }
    #else 
    for (size_t i = 0; i < stk->size; i++) {
        printf(TYPEELEM " ", stk->data[i]);
    }
    #endif

    printf("\n");

    return err;
}
