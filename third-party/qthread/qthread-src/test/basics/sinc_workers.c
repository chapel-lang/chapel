#include <qthread/qthread.h>
#include <qthread/sinc.h>
#include <assert.h>

int expected[31] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040};

typedef struct {
  int i;
  qt_sinc_t* s;
  int *ret;
} args_t;

static aligned_t fib(void *args_){
  args_t* args = (args_t*)args_;
  if(args->i <= 2){
    *args->ret = 1;
    assert(*args->ret == expected[args->i]);
    qt_sinc_submit(args->s, NULL);
    return 0;
  }
  int x, y;
  qt_sinc_t sinc;
  qt_sinc_init(&sinc, 0, NULL, NULL, 2);

  args_t argsx = { args->i-1, &sinc, &x };
  args_t argsy = { args->i-2, &sinc, &y };

  qthread_fork_copyargs(fib, &argsx, sizeof(args_t), NULL);
  qthread_fork_copyargs(fib, &argsy, sizeof(args_t), NULL);

  qt_sinc_wait(&sinc, NULL);
  *args->ret  = x + y;
  assert(*args->ret == expected[args->i]);
  qt_sinc_submit(args->s, NULL);
  return 0;
}

int main(void){
  assert(qthread_initialize() == 0);
  qt_sinc_t sinc;
  qt_sinc_init(&sinc, 0, NULL, NULL, 1);
  int ret;
  int depth = 20;
  args_t args = { depth, &sinc, &ret };
  qthread_fork_copyargs(fib, &args, sizeof(args_t), NULL);
  qt_sinc_wait(&sinc, NULL);
  assert(ret == expected[depth]);
}

