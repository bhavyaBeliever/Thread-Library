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
void remove(){

}