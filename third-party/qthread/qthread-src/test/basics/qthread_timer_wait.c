#include "qthread/qtimer.h"
#include <qthread/qthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include "argparsing.h"

aligned_t cond[2];

#define taskwait                                                               \
  status &= qthread_readFE(NULL, &cond[0]);                                    \
  status &= qthread_readFE(NULL, &cond[1]);

#define reset                                                                  \
  status &= qthread_empty(&cond[0]);                                           \
  status &= qthread_empty(&cond[1]);

double t_long = 0.1;
double t_short = 0.01;

int _Atomic check = 0;

static aligned_t task_long(void *arg) {
  double secs = t_long;
  qtimer_t t = qtimer_create();
  qtimer_start(t);
  do {
    qthread_yield();
    qtimer_stop(t);
  } while (qtimer_secs(t) < secs);
  atomic_fetch_sub_explicit(&check, 2, memory_order_relaxed);
  qthread_writeF(&cond[0], &cond[0]);
  return 0;
}

static aligned_t task_short(void *arg) {
  double secs = t_short;
  qtimer_t t = qtimer_create();
  qtimer_start(t);
  do {
    qthread_yield();
    qtimer_stop(t);
  } while (qtimer_secs(t) < secs);
  // atomic equivalent of check *= 2;
  int check_local = atomic_load_explicit(&check, memory_order_relaxed);
  while (atomic_compare_exchange_weak_explicit(&check,
                                               &check_local,
                                               2 * check_local,
                                               memory_order_relaxed,
                                               memory_order_relaxed));
  qthread_writeF(&cond[1], &cond[1]);
  return 0;
}

static int test1(void) {
  int status = QTHREAD_SUCCESS;
  reset;
  atomic_fetch_add_explicit(&check, 1, memory_order_relaxed);
  status &= qthread_fork(task_long, NULL, NULL);
  status &= qthread_fork(task_short, NULL, NULL);
  taskwait;
  return status;
}

static void qthread_sleep(double secs) {
  qtimer_t t = qtimer_create();
  qtimer_start(t);
  do {
    qthread_yield();
    qtimer_stop(t);
  } while (qtimer_secs(t) < secs);
  qtimer_destroy(t);
}

static aligned_t task(void *arg) {
  qthread_sleep(t_short);
  // atomic equivalent of check *= 2;
  int check_local = atomic_load_explicit(&check, memory_order_relaxed);
  while (atomic_compare_exchange_weak_explicit(&check,
                                               &check_local,
                                               2 * check_local,
                                               memory_order_relaxed,
                                               memory_order_relaxed));
  return 0;
}

static int test2(void) {
  int status = QTHREAD_SUCCESS;
  aligned_t ret;
  status &= qthread_fork(task, NULL, &ret);
  atomic_fetch_add_explicit(&check, 1, memory_order_relaxed);
  qthread_sleep(t_long);
  atomic_fetch_sub_explicit(&check, 2, memory_order_relaxed);
  qthread_readFF(NULL, &ret);
  return status;
}

static aligned_t task_short_inner(void *arg) {
  double secs = t_short;
  qtimer_t t = qtimer_create();
  qtimer_start(t);
  do {
    qthread_yield();
    qtimer_stop(t);
  } while (qtimer_secs(t) < secs);
  atomic_fetch_add_explicit(&check, 1, memory_order_relaxed);
  qthread_writeF(&cond[1], &cond[1]);
  return 0;
}

static aligned_t task_long_outer(void *arg) {
  int status = QTHREAD_SUCCESS;
  status &= qthread_fork(task_short_inner, NULL, NULL);
  test_check(status == QTHREAD_SUCCESS);
  double secs = t_long;
  qtimer_t t = qtimer_create();
  qtimer_start(t);
  do {
    qthread_yield();
    qtimer_stop(t);
  } while (qtimer_secs(t) < secs);
  atomic_fetch_sub_explicit(&check, 2, memory_order_relaxed);
  qthread_writeF(&cond[0], &cond[0]);
  return 0;
}

static int test3(void) {
  int status = QTHREAD_SUCCESS;
  reset;
  atomic_fetch_add_explicit(&check, 1, memory_order_relaxed);
  status &= qthread_fork(task_long_outer, NULL, NULL);
  taskwait;
  return status;
}

int main(int argc, char *argv[]) {
  int status = QTHREAD_SUCCESS;
  status &= qthread_initialize();
  status &= test1();
  test_check(!atomic_load_explicit(&check, memory_order_relaxed));
  status &= test2();
  test_check(!atomic_load_explicit(&check, memory_order_relaxed));
  status &= test3();
  test_check(!atomic_load_explicit(&check, memory_order_relaxed));
  test_check(status == QTHREAD_SUCCESS);
  return EXIT_SUCCESS;
}

/* vim:set expandtab */
