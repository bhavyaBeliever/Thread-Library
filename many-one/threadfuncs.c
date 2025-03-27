#include "thread.h"

static int initialized = 0;

#define STACK_SIZE 8192
t_tid base_tid = 0;
int counter = 0;   
void timer_handler(int sig){
    abthread *oldThread = getCurrentThread();
    if(oldThread->state == RUNNING)
        oldThread->state = RUNNABLE;
    abthread *newThread = getRunnableThread();
    newThread->state = RUNNING;

    getcontext(oldThread->ucp);
    setcontext(newThread->ucp);
}

void thread_init(){
    signal(SIGALRM, timer_handler);
    abthread *mainThread = (abthread *)malloc(sizeof(abthread));
    if (!mainThread) return;
    mainThread->tid = base_tid ++;
    mainThread->state = RUNNING;
    
    mainThread->ucp = (ucontext_t *)malloc(sizeof(ucontext_t));
    int a;

    mainThread->ucp->uc_link = NULL;
    add(mainThread);
}
void startContextSwitch(){

    timer_t *t = (timer_t *)malloc(sizeof(timer_t));
    timer_create(CLOCK_PROCESS_CPUTIME_ID, NULL, t);
    
    struct itimerspec *alarmspec = (struct itimerspec *)malloc(sizeof(struct itimerspec));
    alarmspec->it_value.tv_nsec = 200000;
    alarmspec->it_interval.tv_nsec = 200000;
    timer_settime(t, 0, alarmspec , NULL);
}

void thread_start()
{
    abthread *threadptr = getCurrentThread();
    threadptr->state = RUNNING;
    threadptr->retval = threadptr->entryPoint(threadptr->args);
    threadptr->state = JOINABLE;
    if (threadptr->waitingThread) threadptr->waitingThread->state = RUNNABLE;
    timer_handler(SIGALRM);
    return;
}

int thread_create(t_tid *t, abthread_attr_t *attr, void *(*entryPoint)(void *), void *arg) {
    if (!initialized) thread_init();

    abthread *newThread = (abthread *)malloc(sizeof(abthread));
    if (!(newThread)) {
        perror("Failed to allocate memory for thread");
        return -1;
    }
    
    newThread->tid = base_tid++;
    newThread->entryPoint = entryPoint;
    newThread->args = arg;
    newThread->waitingThread = NULL;
    newThread->ucp = (ucontext_t *)malloc(sizeof(ucontext_t));
    getcontext(newThread->ucp);
    newThread->ucp->uc_link = NULL;
    newThread->ucp->uc_stack.ss_size = STACK_SIZE;
    newThread->ucp->uc_stack.ss_sp = (char *)malloc(STACK_SIZE);
    if(newThread->ucp->uc_stack.ss_sp == NULL) 
        return -1;
    makecontext(newThread->ucp, thread_start, 0);
    
    
    add(newThread); 
    *t = newThread->tid;

    if (!initialized) startContextSwitch();
    initialized = 1;
    return 0;
}


int thread_join(t_tid t, void **retval) {
    sleep(1);
    threadNode *node = search(&t);

    if(!node) return -1;
    abthread *thread = node->thread;
    if(thread->state != JOINABLE){
        abthread *self = getCurrentThread();
        self->state = WAITING;
        thread->waitingThread = self;
        printf("Ans\n");
        timer_handler(SIGALRM);
    }
    if (retval) {
        *retval = thread->retval;
    }
    int *ans = (int *)(*retval);
    removeNode(node);
    return 0;
}


struct factorialInput {
    int start;
    int end;
};

void* factorial(void *args) {
    struct factorialInput *input = (struct factorialInput*)args;
    int *ans = (int*)malloc(sizeof(int));
    *ans = 1;

    for (int i = input->start; i <= input->end; i++) {
        *ans *= i;
    }

    return ans;
}

int main() {
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Factorial is not defined for negative numbers.\n");
        return 1;
    }

    t_tid t1, t2;
    struct factorialInput input1, input2;
    input1.start = 1;
    input1.end = n / 2;
    input2.start = n / 2 + 1;
    input2.end = n;

    thread_create(&t1, NULL, &factorial, &input1);
    thread_create(&t2, NULL, &factorial, &input2);

    void *returnValue1, *returnValue2;
    int join_status1 = thread_join(t1, &returnValue1);
    if (join_status1 == -1) return 0;
    int join_status2 = thread_join(t2, &returnValue2);
    int *ans1 = (int *)returnValue1;
    int *ans2 = (int *)returnValue2;
    int result = *ans1;
    result *= *ans2;
    printf("Factorial of %d is %d\n", n, result);
    return 0;
}