// #include "calculate.h"
#include "calculator.h"

int add(FILE* fileerr, Stack_t* stk) {
    StackElement_t elem1 = 0;
    int err = POP(*stk, elem1);

    StackElement_t elem2 = 0;
    err |= POP(*stk, elem2);

    err |= PUSH(*stk, elem1 + elem2);

    return err;
}

int sub(FILE* fileerr, Stack_t* stk) {
    StackElement_t elem1 = 0;
    int err = POP(*stk, elem1);

    StackElement_t elem2 = 0;
    err |= POP(*stk, elem2);

    err |= PUSH(*stk, elem2 - elem1);

    return err;
}

int mul(FILE* fileerr, Stack_t* stk) {
    StackElement_t elem1 = 0;
    int err = POP(*stk, elem1);

    StackElement_t elem2 = 0;
    err |= POP(*stk, elem2);

    err |= PUSH(*stk, elem2 * elem1);

    return err;
}

int div(FILE* fileerr, Stack_t* stk) {
    StackElement_t elem1 = 0;
    int err = POP(*stk, elem1);

    StackElement_t elem2 = 0;
    err |= POP(*stk, elem2);

    err |= PUSH(*stk, elem2 / elem1);

    return err;
}

int sqr(FILE* fileerr, Stack_t* stk) {
    StackElement_t elem1 = 0;
    int err = POP(*stk, elem1);
    elem1 = (StackElement_t) floor(sqrt(elem1 * pow(10, 6))) / 1000;

    err |= PUSH(*stk, elem1);

    return err;
}

void Calculate(FILE* fileerr, Stack_t* stk1) {
    char command[10] = " ";
    StackElement_t elem = 0;
    
    while (1) {
            scanf("%s", command);
            if (strcmp("PUSH", command) == 0) {
                scanf(TYPEELEM, &elem);
                PUSH(*stk1, elem);
            }

            else if (strcmp("OUT", command) == 0) {
                POP(*stk1, elem);
                PRINTELEM(elem);
            }

            else if (strcmp("ADD", command) == 0) {
                ADD(*stk1);
            }

            else if (strcmp("SUB", command) == 0) {
                SUB(*stk1);
            }

            else if (strcmp("DIV", command) == 0) {
                DIV(*stk1);
            }

            else if (strcmp("MUL", command) == 0) {
                MUL(*stk1);
            }

            else if (strcmp("SQRT", command) == 0) {
                SQRT(*stk1);
            }

            else if (strcmp("PRINT", command) == 0) {
                    PRINTSTACK(*stk1);
            }

            else if (strcmp("HLT", command) == 0) {
                break;
            }

            else {
                printf("Enter correct command\n");
            }
        }
}