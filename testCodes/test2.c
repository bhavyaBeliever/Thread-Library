#include"../one-one/thread.h"
#include<stdio.h>   
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

t_tid t1, t2, t3;
void *a(void *arg){
    int *x = (int *)malloc(sizeof(int));
    *x = *(int *)arg; 
    *x += 1;
    return (void *)x;
}
void *b(void *arg){
    int *y= (int *)malloc(sizeof(int));
    *y = 5;
    void *xaddr;
    thread_join(t1, &xaddr);
    *y += *(int *)xaddr;
    return (void *)y;
}
void *c(void *arg){
    int *z= (int *)malloc(sizeof(int)); 
    *z = 10;
    void *yaddr;
    thread_join(t2, &yaddr);
    *z += *(int *)yaddr;
    return (void *)z; 
}
/*
only one thread can join the code... 
*/
int main(){
    int *x = (int *)malloc(sizeof(int));
    thread_create(&t1, NULL, &a, x);
    thread_create(&t2, NULL, &b, NULL);
    thread_create(&t3, NULL, &c, NULL);
    void *rv;
//    thread_join(t1, &rv);
//    printf("%d ", *(int *)rv);
//    thread_join(t2, &rv);
//    printf("%d ", *(int *)rv);
    thread_join(t3, &rv);
    printf("%d ", *(int *)rv);
    return 0;
}
