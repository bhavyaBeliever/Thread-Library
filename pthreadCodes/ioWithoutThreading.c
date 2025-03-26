#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct timespec start_time, end_time, io_start_time, io_end_time;

void* io_thread(void* arg) {
    int input;

    clock_gettime(CLOCK_MONOTONIC, &io_start_time);

    printf("Enter an integer: ");
    scanf("%d", &input);

    clock_gettime(CLOCK_MONOTONIC, &io_end_time);

    printf("You entered: %d\n", input);
    return NULL;
}

void* arithmetic_thread(void* arg) {
    int a = 10, b = 20;
    int result = a + b;
    printf("Arithmetic result: %d\n", result);
    return NULL;
}

int main() {
    pthread_t thread1;

    clock_gettime(CLOCK_MONOTONIC, &start_time);

    if (pthread_create(&thread1, NULL, io_thread, NULL) != 0) {
        perror("Failed to create I/O thread");
        return 1;
    }

    pthread_join(thread1, NULL);
    if (pthread_create(&thread1, NULL, arithmetic_thread, NULL) != 0) {
        perror("Failed to create arithmetic thread");
        return 1;
    }

    pthread_join(thread1, NULL);

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    long total_time = (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);

    long io_time = (io_end_time.tv_sec - io_start_time.tv_sec) * 1e9 + (io_end_time.tv_nsec - io_start_time.tv_nsec);

    long execution_time = total_time - io_time;

    printf("Total execution time: %ld nanoseconds\n", total_time);
    printf("I/O time (scanf): %ld nanoseconds\n", io_time);
    printf("Execution time (excluding I/O): %ld nanoseconds\n", execution_time);

    return 0;
}
