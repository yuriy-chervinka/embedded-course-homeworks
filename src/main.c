#include <stdio.h>

#if defined(RUN_HW1_PROG1)
    void run_hw1_prog1(void);
#elif defined(RUN_HW1_PROG2)
    void run_hw1_prog2(void);
#elif defined(RUN_HW1_PROG3)
    void run_hw1_prog3(void);
#elif defined(RUN_HW1_PROG4)
    void run_hw1_prog4(void);
#elif defined(RUN_HW1_PROG5)
    void run_hw1_prog5(void);
#elif defined(RUN_HW2_PROG1)
    void run_hw2_prog1(void);
#elif defined(RUN_HW3_PROG1)
    void run_hw3_prog1(void);
#elif defined(RUN_HW4_PROG1)
    void run_hw4_prog1(void);
#elif defined(RUN_MODULE1_PROJ)
    void run_module1_proj(void);
#endif


void app_main(void) {
    #if defined(RUN_HW1_PROG1)
        run_hw1_prog1();
    #elif defined(RUN_HW1_PROG2)
        run_hw1_prog2();
    #elif defined(RUN_HW1_PROG3)
        run_hw1_prog3();
    #elif defined(RUN_HW1_PROG4)
        run_hw1_prog4();
    #elif defined(RUN_HW1_PROG5)
        run_hw1_prog5();
    #elif defined(RUN_HW2_PROG1)
        run_hw2_prog1();
    #elif defined(RUN_HW3_PROG1)
        run_hw3_prog1();
    #elif defined(RUN_HW4_PROG1)
        run_hw4_prog1();
    #elif defined(RUN_MODULE1_PROJ)
        run_module1_proj();
    #else
        printf("\nNo program selected!\n")
    #endif
}