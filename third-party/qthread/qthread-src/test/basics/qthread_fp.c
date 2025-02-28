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
  float exp;
  float ans;
  aligned_t cond;
};

// https://www.w3resource.com/c-programming-exercises/math/c-math-exercise-24.php
static float taylor_exponential_core(int n, float x) {
  float exp_sum = 1;
  for (int i = n - 1; i > 0; --i) { exp_sum = 1 + x * exp_sum / i; }
  return exp_sum;
}

static aligned_t taylor_exponential(void *arg) {
  struct parts *te = (struct parts *)arg;
  te->ans = taylor_exponential_core(te->length, te->exp);
  return 0;
}

static void checkFloat(void) {
  struct parts teParts = {250, 9.0f, 0.0f};
  int ret = -1;
  qthread_empty(&teParts.cond);

  ret = qthread_fork(taylor_exponential, &teParts, &teParts.cond);
  test_check(ret == QTHREAD_SUCCESS);

  ret = qthread_readFF(NULL, &teParts.cond);
  test_check(ret == QTHREAD_SUCCESS);
}

int main(void) {
  float ans = taylor_exponential_core(250, 9.0);
  float expected = 8103.083984f;
  float rel_error = fabsf(ans - expected) / fabsf(expected);
  float threshold = 1E-7f;
  test_check(rel_error < threshold);

  int status = qthread_initialize();
  test_check(status == QTHREAD_SUCCESS);

  checkFloat();
  return EXIT_SUCCESS;
}
