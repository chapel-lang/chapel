/* 
 * myth_cond.c --- test condition variables
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

myth_mutex_t m[1];
myth_cond_t c[1];
volatile long x, y; /* x : consumed. y : produced */

/* consume n_consumers items, to be consumed
   by the consumers (one by each) */
void produce(long batch_id, long n_consumers) {
  myth_mutex_lock(m);
#if 0
  printf("produce(batch_id=%ld, n_consumers=%ld) x = %ld, y = %ld\n",
	 batch_id, n_consumers, x, y);
#endif
  assert(y == batch_id * n_consumers);
  /* wait until all I have produced so far
     have been consumed */
  while (x != y) {
    myth_cond_wait(c, m);
  }
  /* produce n_consumers items (one for each consumer) */
  y += n_consumers;
  /* wake up all consumers */
  myth_cond_broadcast(c);
  myth_mutex_unlock(m);
}

/* consume an item */
void consume(long batch_id, long n_consumers) {
  myth_mutex_lock(m);
#if 1
  (void)batch_id;
  (void)n_consumers;
#else
  printf("consume(batch_id=%ld, n_consumers=%ld) x = %ld, y = %ld\n",
	 batch_id, n_consumers, x, y);
#endif
  while (x == y) {
    myth_cond_wait(c, m);
  }
  /* consume an item */
  x++;
  if (x == y) {
    /* wake up the producer */
    myth_cond_signal(c);
  }
  myth_mutex_unlock(m);
}

typedef struct {
  long n;
  long n_consumers;
} arg_t;

void * producer(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long n = arg->n;
  long n_consumers = arg->n_consumers;
  long i;
  for (i = 0; i < n; i++) {
    produce(i, n_consumers);
  }
  return 0;
}

void * consumer(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long n = arg->n;
  long n_consumers = arg->n_consumers;
  long i;
  for (i = 0; i < n; i++) {
    consume(i, n_consumers);
  }
  return 0;
}

int main(int argc, char ** argv) {
  long n           = (argc > 1 ? atoi(argv[1]) : 1000);
  long n_consumers = (argc > 2 ? atoi(argv[2]) : 100);
  myth_mutex_init(m, 0);
  myth_cond_init(c, 0);
  x = y = 0;

  arg_t arg[1] = { { n, n_consumers } };
  myth_thread_t cons_tids[n_consumers];
  long i;
  for (i = 0; i < n_consumers; i++) {
    cons_tids[i] = myth_create(consumer, arg);
  }
  myth_thread_t prod_tid = myth_create(producer, arg);
  for (i = 0; i < n_consumers; i++) {
    myth_join(cons_tids[i], 0);
  }
  myth_join(prod_tid, 0);
  if (n * n_consumers == x && y == x) {
    printf("OK: n * n_consumers == x == y == %ld\n", x);
    return 0;
  } else {
    printf("NG: n * n_consumers == %ld, x == %ld, y == %ld\n", 
	   n * n_consumers, x, y);
    return 1;
  }
}
