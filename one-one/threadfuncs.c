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

int futex(int *uaddr, int futex_op, int val) {
    return syscall(SYS_futex, uaddr, futex_op, val, NULL, NULL, 0);
}



int thread_start(void *arg)
{
    abthread *threadptr = (abthread *)arg;
    threadptr->state = RUNNING;
    threadptr->retval = threadptr->entryPoint(threadptr->args);
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


int thread_join(t_tid t, void **retval) {
    threadNode *node = search(&t);
    if(!node) return -1;
    abthread *thread = node->thread;
    futex(&thread->state, FUTEX_WAIT, RUNNING);
    if (retval) {
        *retval = thread->retval;
    }
    int *ans = (int *)(*retval);
    removeNode(node);
    return 0;
}