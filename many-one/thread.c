#include "thread.h"
#include <stdlib.h>
threadPool pool = NULL;

void add(abthread *newThread){
    
    threadNode *newThreadNode = (threadNode *)malloc(sizeof(threadNode));    
    newThreadNode->thread = newThread;
    if (!pool) {
        pool = newThreadNode;
        newThreadNode->previous = pool;
    }   
    else
    {
        pool->previous->next = newThreadNode;
        newThreadNode->previous = pool->previous;
        pool->previous = newThreadNode;
    }
    newThreadNode->next = pool;
    // printf("Added %d \n", newThreadNode->thread->tid);
}
void print_thread_node(){
    threadNode *ptr = pool;
    threadNode *start = NULL;
    while(ptr != start){
        printf("Found in circular list %d \n", ptr->thread->tid);
        if (!start) start = ptr;
        ptr = ptr->next;

    }

}
void removeNode(threadNode *node){
    if (!node) return;
    if (node == node->previous && node->next == node){
        free(node);
        pool = NULL;
        return;
    }
    if (node->next){
        node->next->previous = node->previous;
    }    
    if (node-> previous){
        node->previous->next = node->next;
    }   
    
    return;
}
threadNode *search(t_tid *t){
    threadNode *ptr = pool;
    threadNode *start = NULL;
    while(ptr != start){
        if (!ptr->thread) return NULL;
        if(ptr->thread->tid == *t){
            return ptr;
        }
        if (!start) start = ptr;
        ptr = ptr->next;

    }
    return NULL;
}
abthread *getCurrentThread(){
    if(pool) 
    {
        return pool->thread;
        printf("%d \n", pool->thread->tid);
    }
    return NULL;
}
abthread *getRunnableThread(){
    threadNode *start = NULL, *curr = pool->next;
    while(curr != start){
        if (curr->thread->state == RUNNABLE){
            pool = curr;
            return curr->thread;
        }
        curr = curr->next;
        if(!start) start = pool;
    }
    return NULL;
}