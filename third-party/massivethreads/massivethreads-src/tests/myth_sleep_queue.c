/* 
 * myth_sleep_queue.c --- unit test myth_sleep_queue
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//#include <myth/myth.h>
#include "../src/myth_sleep_queue_func.h"

myth_sleep_queue_t q[1];

typedef struct queue_item {
  struct queue_item * next;
  long val;
  int last;
} queue_item;

typedef struct {
  long n;
  long id;
  long nthreads;
  pthread_t tid;
  queue_item my_item;
  queue_item * last_item;
} arg_t;


void * worker(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long n = arg->n;
  long i;
  arg->my_item.next = 0;
  arg->my_item.val = 0;
  arg->my_item.last = 0;
  queue_item * item = &arg->my_item;
  for (i = 0; i < n; i++) {
    myth_sleep_queue_enq(q, (myth_sleep_queue_item *)item);
    item = (queue_item *)myth_sleep_queue_deq(q);
    assert(item);
    /* overwrite next pointer right after dequeued */
    item->next = (queue_item *)4;
    item->val++;
  }
  if (item) {
    item->last = 1;
  }
  return 0;
}

int main(int argc, char ** argv) {
  long nthreads = (argc > 1 ? atol(argv[1]) : 4);
  long n        = (argc > 2 ? atol(argv[2]) : 1000);
  arg_t args[nthreads];
  long i;

  myth_sleep_queue_init(q);
  for (i = 0; i < nthreads; i++) {
    args[i].n = n;
    args[i].id = i;
    args[i].nthreads = nthreads;
    pthread_create(&args[i].tid, 0, worker, &args[i]);
  }
  for (i = 0; i < nthreads; i++) {
    pthread_join(args[i].tid, 0);
  }
  long vals = 0;
  for (i = 0; i < nthreads; i++) {
    vals += args[i].my_item.val;
    assert(args[i].my_item.last);
  }
  assert(vals == n * nthreads);
  assert(q->head == 0);
  assert(q->tail == 0);
  myth_sleep_queue_destroy(q);
  printf("OK\n");
  return 0;
}
