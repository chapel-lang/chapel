/* 
 * myth_cond.c --- test condition variables
 */

#include <pthread.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t m[1];
pthread_cond_t c[1];
volatile long x, y; /* x : consumed. y : produced */

/* consume n_consumers items, to be consumed
   by the consumers (one by each) */
void produce(long batch_id, long n_consumers) {
  pthread_mutex_lock(m);
#if 0
  printf("produce(batch_id=%ld, n_consumers=%ld) x = %ld, y = %ld\n",
	 batch_id, n_consumers, x, y);
#endif
  assert(y == batch_id * n_consumers);
  /* wait until all I have produced so far
     have been consumed */
  while (x != y) {
    pthread_cond_wait(c, m);
  }
  /* produce n_consumers items (one for each consumer) */
  y += n_consumers;
  /* wake up all consumers */
  pthread_cond_broadcast(c);
  pthread_mutex_unlock(m);
}

/* consume an item */
void consume(long batch_id, long n_consumers) {
  pthread_mutex_lock(m);
#if 1
  (void)batch_id;
  (void)n_consumers;
#else
  printf("consume(batch_id=%ld, n_consumers=%ld) x = %ld, y = %ld\n",
	 batch_id, n_consumers, x, y);
#endif
  while (x == y) {
    pthread_cond_wait(c, m);
  }
  /* consume an item */
  x++;
  if (x == y) {
    /* wake up the producer */
    pthread_cond_signal(c);
  }
  pthread_mutex_unlock(m);
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
  pthread_mutex_init(m, 0);
  pthread_cond_init(c, 0);
  x = y = 0;

  arg_t arg[1] = { { n, n_consumers } };
  pthread_t cons_tids[n_consumers];
  long i;
  for (i = 0; i < n_consumers; i++) {
    pthread_create(&cons_tids[i], 0, consumer, arg);
  }
  pthread_t prod_tid;
  pthread_create(&prod_tid, 0, producer, arg);
  for (i = 0; i < n_consumers; i++) {
    pthread_join(cons_tids[i], 0);
  }
  pthread_join(prod_tid, 0);
  if (n * n_consumers == x && y == x) {
    printf("OK: n * n_consumers == x == y == %ld\n", x);
    return 0;
  } else {
    printf("NG: n * n_consumers == %ld, x == %ld, y == %ld\n", 
	   n * n_consumers, x, y);
    return 1;
  }
}
