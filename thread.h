
#include <ucontext.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>


#define STACK_SIZE 8192

typedef int t_tid;
typedef enum { DETACHED, JOINABLE, JOINED } thread_state;

typedef struct abthread {
	void *retval;             // Return value of the thread
	t_tid tid;                      //unique id of thread
	jmp_buf env;		      //context for setjmp/longjmp-based scheduling	
	thread_state state;	      
	char *stack;		      
	void *(*entryPoint)(void);
	void **args;
} abthread;
typedef struct abthread_attr_t{
	stack_size;
	stack;

}abthread_attr_t;	

int thread_create(t_tid *t, abthread_attr_t* attr, void (*entryPoint)(void), void *arg);
void thread_exit(void *retval);
int thread_join(abthread * thread, void **retval);
void thread_yield();
typedef struct threadNode
{
    abthread *thread;
    struct threadNode *previous;
    struct threadNode *next;    
}threadNode;

typedef struct threadNode *threadPool;  


void add();
void remove();
