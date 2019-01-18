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

/* this test program creates a random dag
   each node of which has a join counter
   initilized with the number of incoming edges,
   and a thread working on each node.
   Each thread waits until the join counter
   becomes zero and runs.
 */

typedef struct {
  long edges_begin;
  long edges_end;
  myth_join_counter_t jc[1];
  long in_degree;
  long out_degree;
} node;

node * V;
long * E;

void * visit_node(void * arg_) {
  node * u = (node *)arg_;
  long i;
  for (i = u->edges_begin; i < u->edges_end; i++) {
    myth_join_counter_dec(V[E[i]].jc);
  }
  return 0;
}

void make_graph(long n, long m) {
  V = (node *)calloc(sizeof(node), n);
  E = (long *)calloc(sizeof(long), m);
  long i;
  unsigned short rg0[3] = { 9, 1, 8 };
  unsigned short rg[3];
  for (i = 0; i < 3; i++) { rg[i] = rg0[i]; }
  /* make sure there is at least one edge pointing to v > 0 */
  long v;
  for (v = 1; v < n; v++) {
    long u = nrand48(rg) % v;
    assert(u < v);
    assert(0 <= u);
    assert(u < v);
    assert(v < n);
    V[u].out_degree++;
    V[v].in_degree++;
  }
  /* generate remaining edges randomly */
  for (i = 0; i < m - (n - 1); i++) {
    long u = nrand48(rg) % n;
    long v = nrand48(rg) % (n - 1);
    if (v >= u) v++;
    if (u > v) { u = u ^ v; v = u ^ v; u = u ^ v; }
    assert(0 <= u);
    assert(u < v);
    assert(v < n);
    V[u].out_degree++;
    V[v].in_degree++;
  }
  /* allocate a region in the Edges array for each node */
  long c = 0;
  long u;
  for (u = 0; u < n; u++) {
    long out = V[u].out_degree;
    long in = V[u].in_degree;
    V[u].edges_begin = c;
    V[u].edges_end = c;
    myth_join_counter_init(V[u].jc, 0, in);
    c += out;
  }
  assert(c == m);
  for (i = 0; i < 3; i++) { rg[i] = rg0[i]; }

  for (v = 1; v < n; v++) {
    long u = nrand48(rg) % v;
    assert(u < v);
    long e = V[u].edges_end++;
    E[e] = v;
  }
  for (i = 0; i < m - (n - 1); i++) {
    long u = nrand48(rg) % n;
    long v = nrand48(rg) % (n - 1);
    if (v >= u) v++;
    if (u > v) { u = u ^ v; v = u ^ v; u = u ^ v; }
    assert(u < v);
    long e = V[u].edges_end++;
    E[e] = v;
  }
  /* check if there is at least an edge from each node,
     except for the last node */
  for (u = 0; u < n; u++) {
    assert(V[u].edges_end - V[u].edges_begin == V[u].out_degree);
  }
  assert(V[0].edges_begin == 0);
  for (u = 0; u < n - 1; u++) {
    assert(V[u].edges_end == V[u+1].edges_begin);
  }
  assert(V[n - 1].edges_end == m);
}

int main(int argc, char ** argv) {
  long n_nodes = (argc > 1 ? atol(argv[1]) : 1000);
  long n_edges = (argc > 2 ? atol(argv[2]) : 10000);
  assert(n_edges >= n_nodes - 1);
  assert(n_nodes > 1 || n_edges == 0);
  make_graph(n_nodes, n_edges);
  myth_create_join_many_ex(0, 0, visit_node, V, 0,
			   0, 0,             sizeof(node), 0,
			   n_nodes);
  printf("OK\n");
  return 0;
}
