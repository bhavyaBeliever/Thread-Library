
#include <ucontext.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>


#define STACK_SIZE 8192

typedef int t_tid;
typedef enum { DETACHED, JOINABLE, JOINED } thread_state;

typedef struct abthread {
	t_tid tid;                 
	jmp_buf env;		      //context for setjmp/longjmp-based scheduling	
	thread_state state;	      
	char *stack;		      
} abthread;
	
	
int thread_create(abthread **new_thread, void (*func)(void));
void thread_exit();

typedef struct threadNode
{
    abthread *thread;
    struct threadNode *previous;
    struct threadNode *next;    
}threadNode;

typedef struct threadNode *threadPool;  


void add();
void remove();
