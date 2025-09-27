#include "main.h"

// #define EMPTY_ELEM 


int main() {
    
    FILE* fileerr = fopen("fileerr.log", "w");
    Stack_t stk1 = {};
    
    do {
        int err = INITSTACK(stk1, 9);
        if (err) {
            // printf("fmasd;jgoiadshg;adsng;ldsang'padsng;nS;G\n");
            break;
        }

        // memset(&stk1 + sizeof(stk1) / 2, -1, sizeof(stk1) / 2);
        // printf("canary: %d\n", stk1.data[0]);

        PUSH(stk1, 10);
        PUSH(stk1, 20);
        PUSH(stk1, 30);
        PUSH(stk1, 100);

        // stk1.data[1] = 25;

        StackElement_t val = 0;
    
        POP(stk1, val);
        PRINTELEM(val);

        POP(stk1, val);
        PRINTELEM(val);

        // TOP(stk1, val);
        // printf("%d\n", val);

        // POP(stk1, val);
        // printf("%d\n", val);

        // printf("%d\n", val);

        // StackDump(fileerr, &stk1, 0);

        DTOR(stk1);
    } while (0);
    
    fclose(fileerr);
}