#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <qthread/qthread.h>

// https://www.geeksforgeeks.org/comparison-float-value-c/
// https://dotnettutorials.net/lesson/taylor-series-using-recursion-in-c/
// https://www.studytonight.com/c/programs/important-concepts/sum-of-taylor-series

struct parts
{
  int length;
  double exp;
  double ans;
  aligned_t cond;
};

// https://www.w3resource.com/c-programming-exercises/math/c-math-exercise-24.php
static double taylor_exponential_core(int n, double x)
{
  double exp_sum = 1;
  for (int i = n - 1; i > 0; --i)
  {
    exp_sum = 1 + x * exp_sum / i;
    qthread_yield();
  }
  return exp_sum;
}

static aligned_t taylor_exponential(void *arg)
{
  struct parts *te = (struct parts *)arg;
  double exp = te->exp;
  double length = te->length;
  te->ans = taylor_exponential_core(te->length, te->exp);
  return 0;
}

static void startQthread(struct parts *teParts)
{
  qthread_empty(&teParts->cond);

  int ret = qthread_fork(taylor_exponential, teParts, &teParts->cond);
  assert(ret == QTHREAD_SUCCESS);
}

static aligned_t checkDoubleAsQthreads()
{
  struct parts teParts1 = {250, 9.0, 0.0};
  struct parts teParts2 = {50, 3.0, 0.0};
  struct parts teParts3 = {150, 11.0, 0.0};

  startQthread(&teParts1);
  startQthread(&teParts2);
  startQthread(&teParts3);

  int ret = qthread_readFF(NULL, &teParts1.cond);
  assert(ret == QTHREAD_SUCCESS);

  ret = qthread_readFF(NULL, &teParts2.cond);
  assert(ret == QTHREAD_SUCCESS);

  ret = qthread_readFF(NULL, &teParts3.cond);
  assert(ret == QTHREAD_SUCCESS);

  assert(teParts1.ans == 8103.0839275753824);

  assert(teParts2.ans == 20.085536923187668);

  assert(teParts3.ans == 59874.141715197809);

  return 0;
}

static void checkDoubleAsQthread()
{
  int ret = -1;
  struct parts teParts = {250, 9.0, 0.0};
  qthread_empty(&teParts.cond);

  ret = qthread_fork(taylor_exponential, &teParts, &teParts.cond);
  assert(ret == QTHREAD_SUCCESS);

  ret = qthread_readFF(NULL, &teParts.cond);
  assert(ret == QTHREAD_SUCCESS);

  assert(teParts.ans == 8103.0839275753824);
}

static void checkDouble()
{
  double ans = taylor_exponential_core(250, 9.0);
  assert(ans == 8103.0839275753824);
}

int main(void)
{
  checkDouble();

  int status = qthread_initialize();
  assert(status == QTHREAD_SUCCESS);

  checkDoubleAsQthread();
  checkDoubleAsQthreads();
  return EXIT_SUCCESS;
}