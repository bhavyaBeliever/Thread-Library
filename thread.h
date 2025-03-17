typedef int t_tid;
typedef struct thread{
    t_tid tid;
    
};
typedef struct threadNode
{
    thread *thread;
    struct threadNode *previous;
    struct threadNode *next;    
}threadNode;

typedef struct threadNode *threadPool;  


void add();
void remove();
