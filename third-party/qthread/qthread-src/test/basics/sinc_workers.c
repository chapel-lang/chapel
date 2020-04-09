#include <qthread/qthread.h>
#include <qthread/sinc.h>
#include <assert.h>

typedef struct {
  int i;
  qt_sinc_t* s;
  int *ret;
} args_t;

aligned_t fib(void *args_){
  args_t* args = (args_t*)args_;
  if(args->i < 2){
    *args->ret = 1;
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
  qt_sinc_submit(args->s, NULL);
  *args->ret  = x + y;
  return 0;
}

int main(){
  assert(qthread_initialize() == 0);
  qt_sinc_t sinc;
  qt_sinc_init(&sinc, 0, NULL, NULL, 1);
  int ret;
  args_t args = { 30, &sinc, &ret };
  qthread_fork_copyargs(fib, &args, sizeof(args_t), NULL);
  qt_sinc_wait(&sinc, NULL);
  printf("%d\n", ret);
}

