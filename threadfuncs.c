#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "thread.h"
#include <linux/futex.h>     
#include <sys/syscall.h>      
#include <unistd.h>

#define _SCHED_H 1
#define __USE_GNU 1
#include <bits/sched.h>

#define STACK_SIZE 8192

t_tid thread_count = 0;
int counter = 0;   

static inline int futex(int *uaddr, int futex_op, int val) {
    return syscall(SYS_futex, uaddr, futex_op, val, NULL, NULL, 0);
}



int thread_start(void *arg)
{
    abthread *threadptr = (abthread *)arg;
    threadptr->state = RUNNING;
    threadptr->retval = threadptr->entryPoint(threadptr->args);
    printf("Start\n");
    threadptr->state = JOINABLE;
    if (futex(&threadptr->state, FUTEX_WAKE, 1) == 0)
        threadptr->state = JOINED;
    return 0;
}

// Function to create a new thread
int thread_create(t_tid *t, abthread_attr_t *attr, void *(*entryPoint)(void *), void *arg) {
    abthread *newThread = (abthread *)malloc(sizeof(abthread));
    if (!(newThread)) {
        perror("Failed to allocate memory for thread");
        return -1;
    }

    newThread->tid = ++thread_count;
    newThread->entryPoint = entryPoint;
    newThread->args = arg;
    newThread->stack = malloc(STACK_SIZE);
    if (!(newThread)->stack) {
        perror("Failed to allocate stack");
        free(newThread);
        return -1;
    }
    
    *t = newThread->tid;
    int thread_id = clone(&thread_start, newThread->stack+STACK_SIZE, SIGCHLD |CLONE_SIGHAND|CLONE_FS|CLONE_VM|CLONE_FILES | CLONE_THREAD, newThread, NULL);
    if (thread_id > 0) {
        add(newThread);
    }
    return 0;
}


int thread_join(t_tid *t, void **retval) {
    threadNode *node = search(t);
    if(!node) return -1;
    abthread *thread = node->thread;
    futex(&thread->state, FUTEX_WAIT, RUNNING);
    if (retval) {
        *retval = thread->retval;
    }
    int *ans = (int *)(*retval);
    // removeNode(node);
    printf("Returned %d\n", *ans);
    return 0;
}

// void thread_exit(void *retval) {
//     if (!current_thread) return;

//     printf("Thread %d exiting...\n", current_thread->tid);
//     current_thread->state = TERMINATED;
//     current_thread->retval = retval;

//     abthread *temp = thread_head, *prev = NULL;

//     if (current_thread == thread_head) {
//         if (thread_head->next == thread_head) {  // Last thread
//             thread_head = NULL;
//         } else {
//             while (temp->next != thread_head) temp = temp->next;
//             thread_head = current_thread->next;
//             temp->next = thread_head;
//         }
//     } else {
//         temp = thread_head;
//         while (temp != current_thread) {
//             prev = temp;
//             temp = temp->next;
//         }
//         prev->next = current_thread->next;
//     }

//     free(current_thread->stack);
    
//     if (thread_head) {
//         abthread *next_thread = thread_head;
//         free(current_thread);
//         current_thread = next_thread;
//         longjmp(current_thread->env, 1);
//     } else {
//         printf("No more threads to run. Exiting...\n");
//         exit(0);
//     }
// }

// void thread_yield() {
//     if (!current_thread || !thread_head) {
//         printf("No more threads to yield.\n");
//         return;
//     }

//     abthread *prev_thread = current_thread;
//     abthread *next_thread = current_thread->next;

//     if (current_thread == prev_thread) {
//         printf("Deadlock detected: All threads stuck.\n");
//         return;
//     }

//     if (setjmp(prev_thread->env) == 0) {
//         longjmp(current_thread->env, 1);
//     }
// }
