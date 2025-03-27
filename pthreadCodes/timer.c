#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

void handler(int sig) {
    printf("Timer expired!\n");
}

int main() {
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;

    // Set up the signal handler
    signal(SIGALRM, handler);

    // Create the timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerid;
    
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        return 1;
    }
    
    // Set the timer to expire after 2 seconds
    its.it_value.tv_sec = 2;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        return 1;
    }
    
    while (1)
    {       
        /* code */
    }
    pause();

    return 0;
}