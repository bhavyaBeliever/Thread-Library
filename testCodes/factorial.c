#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct factorialInput {
    int start;
    int end;
};

void* factorial(void *args) {
    struct factorialInput *input = (struct factorialInput*)args;
    int *ans = (int*)malloc(sizeof(int));
    *ans = 1;

    for (int i = input->start; i <= input->end; i++) {
        *ans *= i;
    }

    return ans;
}

int main() {
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Factorial is not defined for negative numbers.\n");
        return 1;
    }

    pthread_t t1, t2;
    struct factorialInput input1, input2;
    input1.start = 1;
    input1.end = n / 2;
    input2.start = n / 2 + 1;
    input2.end = n;

    pthread_create(&t1, NULL, &factorial, &input1);
    pthread_create(&t2, NULL, &factorial, &input2);

    void *returnValue1, *returnValue2;
    pthread_join(t1, &returnValue1);
    pthread_join(t2, &returnValue2);

    int *ans1 = (int *)returnValue1;
    int *ans2 = (int *)returnValue2;
    int result = (*ans1) * (*ans2);

    printf("Factorial of %d is %d\n", n, result);

    free(ans1);
    free(ans2);

    return 0;
}