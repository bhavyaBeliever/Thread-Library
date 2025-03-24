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
        if(ptr->thread->tid == *t){
            return ptr;
        }
        if (!start) start = ptr;
        ptr = ptr->next;

    }
    return NULL;
}