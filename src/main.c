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
#elif defined(RUN_MODULE_1_PROJ)
    void run_module_1_proj(void);
#elif defined(RUN_MODULE_2_1)
    void run_module_2_1(void);
#elif defined(RUN_MODULE_2_2_PROG_1)
    void run_module_2_2_prog_1(void);
#elif defined(RUN_MODULE_2_2_PROG_2)
    void run_module_2_2_prog_2(void);
#elif defined(RUN_MODULE_2_3)
    void run_module_2_3(void);
#elif defined(RUN_MODULE_2_4)
    void run_module_2_4(void);
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
    #elif defined(RUN_MODULE_1_PROJ)
        run_module_1_proj();
    #elif defined(RUN_MODULE_2_1)
        run_module_2_1();
    #elif defined(RUN_MODULE_2_2_PROG_1)
        run_module_2_2_prog_1();
    #elif defined(RUN_MODULE_2_2_PROG_2)
        run_module_2_2_prog_2();
    #elif defined(RUN_MODULE_2_3)
        run_module_2_3();
    #elif defined(RUN_MODULE_2_4)
        run_module_2_4();
    #else
        printf("\nNo program selected!\n")
    #endif
}