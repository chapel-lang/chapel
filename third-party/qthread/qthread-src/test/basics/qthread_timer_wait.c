#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <qthread/qthread.h>
#include "qthread/qtimer.h"

aligned_t cond[2];

#define taskwait \
    status &= qthread_readFE(NULL, &cond[0]); \
    status &= qthread_readFE(NULL, &cond[1]);


#define reset \
    status &= qthread_empty(&cond[0]); \
    status &= qthread_empty(&cond[1]);

double t_long  = 0.1;
double t_short = 0.01;

int check = 0;

aligned_t task_long(void *arg) {
    double secs = t_long;
    qtimer_t t = qtimer_create();
    qtimer_start(t);
    do {
        qthread_yield();
        qtimer_stop(t);
    } while (qtimer_secs(t) < secs);
    check -= 2;
    qthread_writeF(&cond[0], &cond[0]);
    return 0;
}

aligned_t task_short(void *arg) {
    double secs = t_short;
    qtimer_t t = qtimer_create();
    qtimer_start(t);
    do {
        qthread_yield();
        qtimer_stop(t);
    } while (qtimer_secs(t) < secs);
    check *= 2;
    qthread_writeF(&cond[1], &cond[1]);
    return 0;
}

int test1() {
    int status = QTHREAD_SUCCESS;   
    reset;
    check += 1;
    status &= qthread_fork(task_long, NULL, NULL);    
    status &= qthread_fork(task_short, NULL, NULL);
    taskwait;
    return status;
}

void qthread_sleep(double secs) {
  qtimer_t t = qtimer_create();
  qtimer_start(t);
  do {
    qthread_yield();
    qtimer_stop(t);
  } while (qtimer_secs(t) < secs);
  qtimer_destroy(t);
}

aligned_t task(void* arg) {
  qthread_sleep(t_short);
  check *= 2;
  return 0;
}

int test2() {
  aligned_t ret;
  qthread_fork(task, NULL, &ret);
  check += 1;
  qthread_sleep(t_long);
  check -= 2;
  qthread_readFF(NULL, &ret);
}

aligned_t task_short_inner(void *arg) {
    double secs = t_short;
    qtimer_t t = qtimer_create();
    qtimer_start(t);
    do {
        qthread_yield();
        qtimer_stop(t);
    } while (qtimer_secs(t) < secs);
    check += 1;
    qthread_writeF(&cond[1], &cond[1]);
    return 0;
}

aligned_t task_long_outer(void *arg) {
    int status = QTHREAD_SUCCESS;
    status &= qthread_fork(task_short_inner, NULL, NULL);
    assert(status == QTHREAD_SUCCESS);
    double secs = t_long;
    qtimer_t t = qtimer_create();
    qtimer_start(t);
    do {
        qthread_yield();
        qtimer_stop(t);
    } while (qtimer_secs(t) < secs);
    check -= 2;
    qthread_writeF(&cond[0], &cond[0]);
    return 0;
}

int test3() {
    int status = QTHREAD_SUCCESS;
    reset;
    check += 1;
    status &= qthread_fork(task_long_outer, NULL, NULL);    
    taskwait;
    return status;
}

int main(int argc, char *argv[]) {
    int status = QTHREAD_SUCCESS;
    status &= qthread_initialize();
    status &= test1();
    assert(!check);
    status &= test2();
    assert(!check);
    status &= test3();
    assert(!check);
    assert(status == QTHREAD_SUCCESS);
    return EXIT_SUCCESS;
}

/* vim:set expandtab */
