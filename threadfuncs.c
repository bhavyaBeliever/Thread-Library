#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "thread.h"

#define STACK_SIZE 8192

int thread_count = 0;
abthread *current_thread = NULL;  // Pointer to the currently running thread
abthread *thread_head = NULL;     // Circular linked list head

// Function to create a new thread
int thread_create(abthread **new_thread, void (*func)(void)) {
    *new_thread = (abthread *)malloc(sizeof(abthread));
    if (!(*new_thread)) {
        perror("Failed to allocate memory for thread");
        return -1;
    }

    (*new_thread)->id = ++thread_count;
    (*new_thread)->state = READY;
    (*new_thread)->stack = malloc(STACK_SIZE);
    if (!(*new_thread)->stack) {
        perror("Failed to allocate stack");
        free(*new_thread);
        return -1;
    }

    // Set up the stack frame for the new thread
    if (setjmp((*new_thread)->env) == 0) {
        // Modify the stack pointer to point to the top of the allocated stack
        void *stack_top = (char *)(*new_thread)->stack + STACK_SIZE;
        asm volatile("mov %0, %%rsp" :: "r"(stack_top)); // Set stack pointer
        func();  // Run the function
        thread_exit(); // Exit when function completes
    }

    // Add the thread to the circular linked list
    if (!thread_head) {
        thread_head = *new_thread;
        (*new_thread)->next = thread_head;
    } else {
        abthread *temp = thread_head;
        while (temp->next != thread_head) {
            temp = temp->next;
        }
        temp->next = *new_thread;
        (*new_thread)->next = thread_head;
    }

    return 0;
}


void thread_exit() {
    if (!current_thread) {
        printf("Error: No thread is currently running.\n");
        return;
    }

    printf("Thread %d exiting...\n", current_thread->id);
    current_thread->state = TERMINATED;

    // Remove the thread from the circular linked list
    abthread *temp = thread_head;
    abthread *prev = NULL;

    if (current_thread == thread_head) {
        if (thread_head->next == thread_head) {  // Only one thread exists
            thread_head = NULL;
        } else {
            while (temp->next != thread_head) {  // Find the last node
                temp = temp->next;
            }
            thread_head = current_thread->next;
            temp->next = thread_head;
        }
    } else {
        temp = thread_head;
        while (temp != current_thread) {
            prev = temp;
            temp = temp->next;
        }
        prev->next = current_thread->next;
    }

    free(current_thread->stack);
    free(current_thread);

    if (thread_head) {
        current_thread = thread_head;  // Switch to next thread
        longjmp(current_thread->env, 1);
    } else {
        printf("No more threads to run. Exiting...\n");
        exit(0);
    }
}
