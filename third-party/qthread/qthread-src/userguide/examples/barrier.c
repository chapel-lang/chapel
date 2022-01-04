#include <stdio.h>
#include <qthread/qthread.h>
#include <qthread/barrier.h>

const int n = 30;
qt_barrier_t *wait_on_me;
aligned_t master = 0;

static aligned_t incr(void *arg){
    qthread_incr(&master, 1);
    qt_barrier_enter_id(wait_on_me, 0);
    return 0;
}

int main(){
    qthread_initialize();
    size_t i;
    wait_on_me = qt_barrier_create(n+1, REGION_BARRIER);
    for(i=0; i<n; i++)
        qthread_fork(incr, NULL, NULL);
    qt_barrier_enter(wait_on_me); 
    qt_barrier_destroy(wait_on_me);
    printf("Incremented %d times\n",(int)master);
    return 0;
}

