#include <ucontext.h>
#include <setjmp.h>
#include <signal.h>
#include <stlib.h>


#define STACK_SIZE 8192

typedef enum { READY, RUNNING, BLOCKED, TERMINATED } thread_state;

typedef struct abthread {
	int tid;                      //unique id of thread
	jmp_buf env;		      //context for setjmp/longjmp-based scheduling	
	thread_state state;	      //Current state of the thread
	void *stack;		      // Stack memory for the thread
	struct abthread * next;	      // Pointer to the next thread for scheduling (Circular linked list)
} abthread;
	
	
int thread_create(thread **new_thread, void (*func)(void));
void thread_exit();
