#include <stdio.h>
#include <qthread/qthread.h>

const int n = 30;
aligned_t master = 0;

static aligned_t incr(void *arg){
    qthread_incr(&master, 1);
    return 0;
}

int main(){
    qthread_initialize();
    aligned_t rets[n];
    size_t i;
    for(i=0; i<n; i++)
        qthread_fork(incr, NULL, rets + i);
    
    for(i=0; i<n; i++)
        qthread_readFF(NULL, rets + i);
    
    printf("Incremented %d times\n",(int)master);
    return 0;
}
