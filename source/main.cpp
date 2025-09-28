#include "main.h"

// #define EMPTY_ELEM 

int main() {
    
    Stack_t stk1 = {};
    FILE* fileerr = fopen("fileerr.log", "w");

    do {
        int err = INITSTACK(stk1, 9); // init for debug

        // int err = INITSTACK(stk1, 2); // BAD PUSH

        // printf("err = %d\n", err);
        if (err == ERR_CAPASITY || err == ERR_DATA_ADRESS) {
            // printf("fmasd;jgoiadshg;adsng;ldsang'padsng;nS;G\n");
            break;
        }

        //---------------------------------------------------------------------------------------------------------------------------------------------------
        // memset(&stk1, -1, sizeof(stk1));
        // printf("canary: %d\n", stk1.data[0]);    

        // for (int i = 0; i < 5; i++) { // Verify PUSH
        //     PUSH(stk1, 10);
        // }
        //---------------------------------------------------------------------------------------------------------------------------------------------------
        
        CALCULATE(stk1);

        // stk1.data[1] = 25;

        StackElement_t val = 0;

        //---------------------------------------------------------------------------------------------------------------------------------------------------
        // for (int i = 0; i < 10; i++) { // Verify POP
        //     POP(stk1, val);
        // }

        // REALLOC(stk1);
        // printf("%d", stk1->capasity);   
        //---------------------------------------------------------------------------------------------------------------------------------------------------


        POP(stk1, val);
        PRINTELEM(val);

        POP(stk1, val);
        PRINTELEM(val);

        StackDump(fileerr, &stk1, 1024);

        DTOR(stk1);
    } while (0);
    
    fclose(fileerr);
}