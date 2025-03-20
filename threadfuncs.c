#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "thread.h"

#define _SCHED_H 1
#define __USE_GNU 1
#include <bits/sched.h>

#define STACK_SIZE 8192

t_tid thread_count = 0;
int counter = 0;
abthread *current_thread = NULL;
abthread *thread_head = NULL;     
int thread_start(abthread *threadptr)
{
    int rv = threadptr->entryPoint();
    return rv;
    futex(uaddr, futex_wake, 1);
}
// Function to create a new thread
int thread_create(t_tid *t, abthread_attr_t *attr, void (*entryPoint)(void), void *arg) {
    abthread *newThread = (abthread *)malloc(sizeof(abthread));
    if (!(newThread)) {
        perror("Failed to allocate memory for thread");
        return -1;
    }

    newThread->tid = ++thread_count;
    newThread->stack = malloc(STACK_SIZE);
    newThread->entryPoint = &entryPoint;
    newThread->args = &arg;
    if (!(newThread)->stack) {
        perror("Failed to allocate stack");
        free(newThread);
        return -1;
    }
    
    int thread_id = clone(thread_start, newThread->stack+STACK_SIZE, SIGCHLD |CLONE_SIGHAND|CLONE_FS|CLONE_VM|CLONE_FILES | CLONE_THREAD, newThread, NULL);
    // newThread->state = DETACHED;

    // Set up the stack frame for the new thread
    // Add the thread to the circular linked list


    return 0;
}


int thread_join(abthread *thread, void **retval) {
    if (!thread) return -1;

    while (thread->state != JOINED) {
        if (thread_head) {  // If no threads are left, exit the loop
            printf("All threads have exited.\n");
            return 0;
        }

        thread_yield(); 
    }
    futex(uaddr, futex_wait, thread->tid);
    if (retval) {
        *retval = thread->retval;
    }

    return 0;
}

void thread_exit(void *retval) {
    if (!current_thread) return;

    printf("Thread %d exiting...\n", current_thread->tid);
    current_thread->state = TERMINATED;
    current_thread->retval = retval;

    abthread *temp = thread_head, *prev = NULL;

    if (current_thread == thread_head) {
        if (thread_head->next == thread_head) {  // Last thread
            thread_head = NULL;
        } else {
            while (temp->next != thread_head) temp = temp->next;
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
    
    if (thread_head) {
        abthread *next_thread = thread_head;
        free(current_thread);
        current_thread = next_thread;
        longjmp(current_thread->env, 1);
    } else {
        printf("No more threads to run. Exiting...\n");
        exit(0);
    }
}

void thread_yield() {
    if (!current_thread || !thread_head) {
        printf("No more threads to yield.\n");
        return;
    }

    abthread *prev_thread = current_thread;
    abthread *next_thread = current_thread->next;

    if (current_thread == prev_thread) {
        printf("Deadlock detected: All threads stuck.\n");
        return;
    }

    if (setjmp(prev_thread->env) == 0) {
        longjmp(current_thread->env, 1);
    }
}
