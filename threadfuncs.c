#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "thread.h"

#define _SCHED_H 1
#define __USE_GNU 1
#include <bits/sched.h>

#define STACK_SIZE 8192

t_tid thread_count = 0;
abthread *current_thread = NULL;
abthread *thread_head = NULL;     

// Function to create a new thread
int thread_create(t_tid *t, void (*entryPoint)(void), void *arg) {
    abthread *newThread = (abthread *)malloc(sizeof(abthread));
    if (!(newThread)) {
        perror("Failed to allocate memory for thread");
        return -1;
    }

    newThread->tid = ++thread_count;
    newThread->stack = malloc(STACK_SIZE);
    if (!(newThread)->stack) {
        perror("Failed to allocate stack");
        free(newThread);
        return -1;
    }
    
    int thread_id = clone(&entryPoint, newThread->stack+STACK_SIZE, SIGCHLD |CLONE_SIGHAND|CLONE_FS|CLONE_VM|CLONE_FILES | CLONE_THREAD, arg, NULL);
    newThread->state = DETACHED;

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
