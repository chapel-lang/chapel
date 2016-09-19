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
void produce(long n_producers, long n_consumers, long buffer_size) {
  (void)n_producers;
  (void)n_consumers;
  myth_mutex_lock(m);
  /* wait until all I have produced so far
     have been consumed */
  while (!(y - x < buffer_size)) {
    myth_cond_wait(c, m);
  }
  /* produce n_consumers items (one for each consumer) */
  y++;
  /* wake up all consumers */
  myth_cond_broadcast(c);
  myth_mutex_unlock(m);
}

/* consume an item */
void consume(long batch_id, long n_consumers) {
  (void)batch_id;
  (void)n_consumers;
  myth_mutex_lock(m);
  while (y == x) {
    myth_cond_wait(c, m);
  }
  /* consume an item */
  x++;
  myth_cond_broadcast(c);
  myth_mutex_unlock(m);
}

typedef struct {
  long n;
  long n_consumers;
  long n_producers;
  long buffer_size;
} arg_t;

void * producer(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long n = arg->n;
  long n_consumers = arg->n_consumers;
  long buffer_size = arg->buffer_size;
  long i;
  for (i = 0; i < n; i++) {
    produce(i, n_consumers, buffer_size);
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
  long n_producers = (argc > 2 ? atoi(argv[2]) : 100);
  long n_consumers = (argc > 3 ? atoi(argv[3]) : 100);
  long buffer_size = (argc > 4 ? atoi(argv[4]) : 10);
  myth_mutex_init(m, 0);
  myth_cond_init(c, 0);
  x = y = 0;

  arg_t arg[1] = { { n, n_producers, n_consumers, buffer_size } };
  myth_thread_t prod_tids[n_producers];
  myth_thread_t cons_tids[n_consumers];
  long i;
  for (i = 0; i < n_consumers; i++) {
    cons_tids[i] = myth_create(consumer, arg);
  }
  for (i = 0; i < n_producers; i++) {
    prod_tids[i] = myth_create(producer, arg);
  }
  for (i = 0; i < n_consumers; i++) {
    myth_join(cons_tids[i], 0);
  }
  for (i = 0; i < n_producers; i++) {
    myth_join(prod_tids[i], 0);
  }
  if (n * n_producers == x && y == x) {
    printf("OK: n * n_producers == x == y == %ld\n", x);
    return 0;
  } else {
    printf("NG: n * n_producers == %ld, x == %ld, y == %ld\n", 
	   n * n_producers, x, y);
    return 1;
  }
}
