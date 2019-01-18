/* 
 * myth_cond.c --- test condition variables
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

myth_mutex_t m[1];
myth_cond_t c[1];
long x, y;

/* wait until y - x becomes 0 increment y
   (so y - x becomes 1) */
void produce(long z) {
  myth_mutex_lock(m);
  assert(y == z);
  while (x != y) {
    myth_cond_wait(c, m);
  }
  y++;
  myth_cond_signal(c);
  myth_mutex_unlock(m);
}

/* wait until y - x becomes 1 increment x 
   (so y - x becomes 0) */
void consume(long z) {
  myth_mutex_lock(m);
  assert(x == z);
  while (x == y) {
    myth_cond_wait(c, m);
  }
  assert(y == z + 1);
  x++;
  myth_cond_signal(c);
  myth_mutex_unlock(m);
}

typedef struct {
  long n;
} arg_t;

void * producer(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long n = arg->n;
  long i;
  for (i = 0; i < n; i++) {
    produce(i);
  }
  return 0;
}

void * consumer(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long n = arg->n;
  long i;
  for (i = 0; i < n; i++) {
    consume(i);
  }
  return 0;
}

int main(int argc, char ** argv) {
  long n = (argc > 1 ? atoi(argv[1]) : 10000);
  myth_mutex_init(m, 0);
  myth_cond_init(c, 0);
  x = y = 0;

  arg_t arg[1] = { { n } };
  myth_thread_t cons_tid = myth_create(consumer, arg);
  myth_thread_t prod_tid = myth_create(producer, arg);
  myth_join(prod_tid, 0);
  myth_join(cons_tid, 0);
  if (x == n && y == n) {
    printf("OK: n == y == z == %ld\n", n);
    return 0;
  } else {
    printf("NG: n == %ld, x == %ld, y == %ld\n", x, y, n);
    return 1;
  }
}
