
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <myth/myth.h>

typedef struct node {
  long v;
  struct node * c[2];
  int leaf;
} node;

node * make_leaf(long v) {
  node * t = malloc(sizeof(node));
  t->v = v;
  t->c[0] = t->c[1] = 0;
  t->leaf = 1;
  return t;
}

node * make_node(long v, node * l, node * r) {
  node * t = malloc(sizeof(node));
  t->v = v;
  t->c[0] = l;
  t->c[1] = r;
  t->leaf = 0;
  return t;
}

void free_leaf(node * t) {
  assert(t->leaf);
  assert(t->c[0] == 0);
  assert(t->c[1] == 0);
  free(t);
}

void free_node(node * t) {
  assert(t->leaf == 0);
  assert(t->c[0]);
  assert(t->c[1]);
  free(t);
}

typedef struct {
  long a;
  long b;
  node * t;
} arg_t;
 
void * mk_tree(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long a = arg->a, b = arg->b;
  if (b - a == 1) {
    arg->t = make_leaf(1);
  } else {
    long c = (a + b) / 2;
    arg_t cargs[2] = { { a, c, 0 }, { c, b, 0 } };
    myth_thread_t tid = myth_create(mk_tree, cargs);
    mk_tree(cargs + 1);
    myth_join(tid, 0);
    arg->t = make_node(cargs[0].t->v + cargs[1].t->v + 1, cargs[0].t, cargs[1].t);
  }
  return 0;
}

void * del_tree(void * arg_) {
  arg_t * arg = (arg_t *)arg_;
  long a = arg->a, b = arg->b;
  node * t = arg->t;
  if (b - a == 1) {
    free_leaf(t);
  } else {
    long c = (a + b) / 2;
    arg_t cargs[2] = { { a, c, t->c[0] }, { c, b, t->c[1] } };
    myth_thread_t tid = myth_create(del_tree, cargs);
    del_tree(cargs + 1);
    myth_join(tid, 0);
    free_node(t);
  }
  return 0;
}

double cur_time() {
  struct timespec ts[1];
  clock_gettime(CLOCK_REALTIME, ts);
  return ts->tv_sec + ts->tv_nsec * 1.0e-9;
}

int main(int argc, char ** argv) {
  long nthreads = (argc > 1 ? atol(argv[1]) : 100000);
  long i;
  for (i = 0; i < 3; i++) {
    arg_t arg[1] = { { 0, nthreads, 0 } };
    double t0 = cur_time();
    myth_thread_t tid = myth_create(mk_tree, arg);
    myth_join(tid, 0);
    double t1 = cur_time();
    assert(arg->t->v == 2 * nthreads - 1);
    tid = myth_create(del_tree, arg);
    myth_join(tid, 0);
    double t2 = cur_time();
    double dt0 = t1 - t0;
    double dt1 = t2 - t1;
    long n_nodes = 2 * nthreads - 1;
    printf("%ld allocates in %.9f sec (%.3f per sec)\n",
	   n_nodes, dt0, n_nodes / dt1);
    printf("%ld frees in %.9f sec (%.3f per sec)\n",
	   n_nodes, dt1, n_nodes / dt1);
  }
  return 0;
}

