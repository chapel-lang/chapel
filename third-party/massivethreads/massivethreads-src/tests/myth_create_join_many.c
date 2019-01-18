
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

typedef struct {
  myth_thread_t id;
  long x;
  long y;
} arg_t;

void * f(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long y = arg->x * arg->x;
  return (void *)y;
}

int main(int argc, char ** argv) {
  long nthreads = (argc > 1 ? atol(argv[1]) : 100);
  arg_t * args = (arg_t *)calloc(sizeof(arg_t), nthreads);
  long i;
  for (i = 0; i < nthreads; i++) {
    args[i].x = i + 1;
  }
  int r = myth_create_join_many_ex(&args[0].id, 0, f, args, (void **)&args[0].y,
				   sizeof(arg_t), 0, sizeof(arg_t), sizeof(arg_t),
				   nthreads);
  assert(r == 0);
  for (i = 0; i < nthreads; i++) {
    assert(args[i].y == args[i].x * args[i].x);
  }
  printf("OK\n");
  return 0;
}

