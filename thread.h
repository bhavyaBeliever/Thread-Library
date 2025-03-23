
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <ucontext.h>
#include <linux/futex.h>      /* Definition of FUTEX_* constants */
#include <sys/syscall.h>      /* Definition of SYS_* constants */

#define STACK_SIZE 8192

typedef int t_tid;
typedef enum { RUNNING, JOINABLE, JOINED } thread_state;

typedef struct abthread {
	void *retval;             // Return value of the thread
	t_tid tid;                      //unique id of thread
	jmp_buf env;		   
	int state;	      
	char *stack;		      
	void *(*entryPoint)(void *);
	void **args;	
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
void thread_exit(void *retval);
int thread_join(t_tid *t, void **retval);
void thread_yield();



void add(abthread *newThread);
void removeNode(threadNode *node);
threadNode *search(t_tid *t);