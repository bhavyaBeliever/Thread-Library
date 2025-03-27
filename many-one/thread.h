
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <ucontext.h>
#include <linux/futex.h>      /* Definition of FUTEX_* constants */
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <sys/types.h>
#define _SCHED_H 1
#define __USE_GNU 1
#include <bits/sched.h>
#define STACK_SIZE 8192

typedef int t_tid;
typedef enum { RUNNABLE, RUNNING, WAITING, JOINABLE } thread_state;
typedef struct abthread{ 
	void *retval;           
	t_tid tid;
	int state;	      
	char *stack;		      
	void *(*entryPoint)(void *);
	void **args;	
	ucontext_t *ucp;
	struct abthread *waitingThread;
} abthread;
typedef struct abthread_attr_t{
	int stack_size;
	void *stack;
}abthread_attr_t;	

typedef struct threadNode
{
    abthread *thread;
    struct threadNode *previous;
    struct threadNode *next;    
}threadNode;
typedef struct threadNode *threadPool;  

int thread_create(t_tid *t, abthread_attr_t* attr, void *(*entryPoint)(void *), void *arg);
int thread_join(t_tid t, void **retval);


int futex(int *uaddr, int futex_op, int val);

void add(abthread *newThread);
void removeNode(threadNode *node);
threadNode *search(t_tid *t);
void print_thread_node();
abthread *getCurrentThread();
abthread *getRunnableThread();
