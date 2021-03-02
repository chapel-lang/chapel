#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/sinc.h>

const int n = 30;
const int d = 4;

typedef struct v_args_s {
    size_t     depth;
    qt_sinc_t *sinc;
} v_args_t;

static void incr(void* target, const void *src){
    *(int*)target += *(int*)src; 
}

static aligned_t buildAndReduce(void* arg_){
    size_t i;
    v_args_t* arg = (v_args_t*)arg_;
    qt_sinc_t sinc; 
    aligned_t result, initial = 0;
    qt_sinc_init(&sinc, sizeof(aligned_t), &initial, incr, n);
    if(arg->depth > 1){
        v_args_t args = { arg->depth - 1, &sinc };
        for(i=0; i<n; i++)
            qthread_fork(buildAndReduce, &args, NULL);
        qt_sinc_wait(&sinc, &result);
        qt_sinc_submit(arg->sinc, &result);
    } else{
        result = 1;
        qt_sinc_submit(arg->sinc, &result);
    }
    return 0;
}

int main(){
    aligned_t result, initial = 0;
    qthread_initialize();
    qt_sinc_t sinc; 
    qt_sinc_init(&sinc, sizeof(aligned_t), &initial, incr, n);
    v_args_t arg = {d, &sinc};
    size_t i;
    for(i=0; i<n; i++)
        qthread_fork(buildAndReduce, &arg, NULL);
    qt_sinc_wait(&sinc, &result);
    printf("Incremented %d times\n",(int)result);
    return 0;
}

