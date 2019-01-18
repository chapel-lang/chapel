/* 
 * dag.c --- test dag scheduling
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

/* this test program creates a random dag
   each node of which has a join counter
   initilized with the number of incoming edges,
   and a thread working on each node.
   Each thread waits until the join counter
   becomes zero and runs.
 */

typedef struct {
  long x;
  long y;
  myth_join_counter_t jc[1];
} node;

node * V;
long N;

void * visit_node(void * arg_) {
  node * u = (node *)arg_;
  long x = u->x;
  myth_join_counter_wait(u->jc);
  if (x == N - 1) {
    u->y = x;
  } else {
    u->y = V[x + 1].y + x;
  }
  if (x > 0) {
    myth_join_counter_dec(V[x - 1].jc);
  }
  return 0;
}

void make_graph(long n) {
  N = n;
  V = (node *)calloc(sizeof(node), n);
  long i;
  for (i = 0; i < n; i++) {
    V[i].x = i;
    V[i].y = -1;
    myth_join_counter_init(V[i].jc, 0, (i == n - 1 ? 0 : 1));
  }
}

int main(int argc, char ** argv) {
  long n_nodes = (argc > 1 ? atol(argv[1]) : 100);
  make_graph(n_nodes);

  myth_create_join_many_ex(0, 0, visit_node, V, 0,
			   0, 0,             sizeof(node), 0,
			   n_nodes);

  assert(V[0].y == n_nodes * (n_nodes - 1) / 2);
  printf("OK\n");
  return 0;
}
