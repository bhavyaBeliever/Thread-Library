#include "thread.h"
#include <stdlib.h>
threadPool pool = NULL;

void add(abthread *newThread){
    
    threadNode *newThreadNode = (threadNode *)malloc(sizeof(threadNode));    
    newThreadNode->thread = newThread;
    newThreadNode->next = pool;
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
}
void removeNode(threadNode *node){
    node->previous->next = node->next;
    node->next->previous = node->previous;
    free(node);
    return;
}
threadNode *search(t_tid *t){
    threadNode *ptr = pool;
    while(ptr->next != pool){
        if(ptr->thread->tid == *t){
            return ptr;
        }
        ptr = ptr->next;

    }
    return NULL;
}