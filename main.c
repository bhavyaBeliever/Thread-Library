#include <stdio.h>
#include "threadfuncs.c"

#define N 65  // Factorial upper limit

unsigned long fact1 = 1;
unsigned long fact2 = 1;

void func1() {
    int i;
    for (i = 1; i <= N / 10; i++) {
        fact1 *= i;
    }
    printf("Partial factorial (1 to %d): %lu\n", N / 10, fact1);
    thread_exit(NULL);
}

void func2() {
    int i;
    for (i = N / 10 + 1; i <= N; i++) {
        fact2 *= i;
    }
    printf("Partial factorial (%d to %d): %lu\n", N / 10 + 1, N, fact2);
    thread_exit(NULL);
}

int main() {
    abthread *t1, *t2;
    thread_create(&t2, func2);
    printf("Second function executed \n");

    thread_create(&t1, func1);
    printf("First function executed \n");
    
    thread_join(t2, NULL);
    printf("Execution of t2 has finished.\n");

    thread_join(t1, NULL);
    printf("Execution of t1 has finished.\n");


    unsigned long final_result = fact1 * fact2;
    printf("Final factorial of %d: %lu\n", N, final_result);

    return 0;
}
