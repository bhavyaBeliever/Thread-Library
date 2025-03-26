#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


#define _SCHED_H 1
#define __USE_GNU 1
#include <bits/sched.h>

#define STACK_SIZE 4096

int func(void *arg) {
    int count = *((int *)arg);
    printf("Inside func.\n");
    sleep(10*count);
    printf("%d", count);
    printf("Terminating func...\n");

    return 1;
}

int main() {
    printf("This process pid: %u\n", getpid());
    char status_file[] = "/proc/self/status";
    char *child_stack1 = (char *)malloc(STACK_SIZE);
    char *child_stack2 = (char *)malloc(STACK_SIZE);
    char *tmp = child_stack1;
    int thread_pid1, thread_pid2;
    FILE *fp = fopen(status_file, "rb");

    printf("Looking into %s...\n", status_file);

    while(1) {
        char ch = fgetc(fp);
        if(feof(fp)) break;
        printf("%c", ch);
    }
    printf("Creating new thread...\n");
    int *count1 = (int *)malloc(sizeof(int));
    *count1 = 1;
    thread_pid1 = clone(&func, child_stack1+STACK_SIZE, 0 | CLONE_VM , count1, NULL);
    *count1 = 2;
    printf("%d ", *count1);
    
    printf("Done! Thread pid: %d\n", thread_pid1);

    printf("Looking into %s...\n", status_file);

    // while(1) {
    //     char ch = fgetc(fp);
    //     if(feof(fp)) break;
    //     printf("%c", ch);
    // }

    fclose(fp);
    int wstatus;
    waitpid(thread_pid1, &wstatus, __WCLONE);
    printf("Started, %d", wstatus);
    getchar();
    printf("%d ", (child_stack1-tmp));
    return 0;
}