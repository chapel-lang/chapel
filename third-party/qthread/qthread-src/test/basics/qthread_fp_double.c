#include <assert.h>
#include <math.h>
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "argparsing.h"

// https://www.geeksforgeeks.org/comparison-float-value-c/
// https://dotnettutorials.net/lesson/taylor-series-using-recursion-in-c/
// https://www.studytonight.com/c/programs/important-concepts/sum-of-taylor-series

struct parts {
  int length;
  double exp;
  double ans;
  aligned_t cond;
};

// https://www.w3resource.com/c-programming-exercises/math/c-math-exercise-24.php
static double taylor_exponential_core(int n, double x, int yield) {
  double exp_sum = 1;
  for (int i = n - 1; i > 0; --i) {
    exp_sum = 1 + x * exp_sum / i;
    if (yield) qthread_yield();
  }
  return exp_sum;
}

static aligned_t taylor_exponential(void *arg) {
  struct parts *te = (struct parts *)arg;
  te->ans = taylor_exponential_core(te->length, te->exp, 1);
  return 0;
}

static void startQthread(struct parts *teParts) {
  qthread_empty(&teParts->cond);

  int ret = qthread_fork(taylor_exponential, teParts, &teParts->cond);
  test_check(ret == QTHREAD_SUCCESS);
}

static aligned_t checkDoubleAsQthreads(void) {
  struct parts teParts1 = {250, 9.0, 0.0};
  struct parts teParts2 = {50, 3.0, 0.0};
  struct parts teParts3 = {150, 11.0, 0.0};

  startQthread(&teParts1);
  startQthread(&teParts2);
  startQthread(&teParts3);

  int ret = qthread_readFF(NULL, &teParts1.cond);
  test_check(ret == QTHREAD_SUCCESS);

  ret = qthread_readFF(NULL, &teParts2.cond);
  test_check(ret == QTHREAD_SUCCESS);

  ret = qthread_readFF(NULL, &teParts3.cond);
  test_check(ret == QTHREAD_SUCCESS);

  double threshold = 1E-15;

  double expected_1 = 8103.0839275753824;
  double rel_error_1 = fabs(expected_1 - teParts1.ans) / fabs(expected_1);
  test_check(rel_error_1 < threshold);

  double expected_2 = 20.085536923187668;
  double rel_error_2 = fabs(expected_2 - teParts2.ans) / fabs(expected_2);
  test_check(rel_error_2 < threshold);

  double expected_3 = 59874.141715197809;
  double rel_error_3 = fabs(expected_3 - teParts3.ans) / fabs(expected_3);
  test_check(rel_error_3 < threshold);

  return 0;
}

static void checkDoubleAsQthread(void) {
  int ret = -1;
  struct parts teParts = {250, 9.0, 0.0};
  qthread_empty(&teParts.cond);

  ret = qthread_fork(taylor_exponential, &teParts, &teParts.cond);
  test_check(ret == QTHREAD_SUCCESS);

  ret = qthread_readFF(NULL, &teParts.cond);
  test_check(ret == QTHREAD_SUCCESS);

  double expected = 8103.0839275753824;
  double rel_error = fabs(expected - teParts.ans) / fabs(expected);
  test_check(rel_error < 1E-15);
}

static void checkDouble(void) {
  double ans = taylor_exponential_core(250, 9.0, 0);
  double expected = 8103.0839275753824;
  double rel_error = fabs(expected - ans) / fabs(expected);
  test_check(rel_error < 1E-15);
}

int main(void) {
  checkDouble();

  int status = qthread_initialize();
  test_check(status == QTHREAD_SUCCESS);

  checkDoubleAsQthread();
  checkDoubleAsQthreads();
  return EXIT_SUCCESS;
}
