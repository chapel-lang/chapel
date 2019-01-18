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
  long r;
  myth_join_counter_t jc[1];
} node;

node * V1;
long M, N;

#define V(i,j) V1[(i)*N+(j)]

void * visit_node(void * arg_) {
  node * u = (node *)arg_;
  long x = u->x;
  long y = u->y;
  myth_join_counter_wait(u->jc);
  //node (*V)[N] = (node (*)[N])V1;

  if (x == M - 1 || y == N - 1) {
    u->r = 1;
  } else {
    u->r = V(x + 1,y).r + V(x,y + 1).r;
  }
  if (x > 0) {
    myth_join_counter_dec(V(x - 1,y).jc);
  }
  if (y > 0) {
    myth_join_counter_dec(V(x,y - 1).jc);
  }
  return 0;
}

void init_graph(long m, long n, node * V1) {
  long i, j;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      V(i,j).x = i;
      V(i,j).y = j;
      V(i,j).r = -1;
      long c = (i < m - 1) + (j < n - 1);
      myth_join_counter_init(V(i,j).jc, 0, c);
    }
  }
}

node * make_graph(long m, long n) {
  node * V1 = (node *)calloc(sizeof(node), m * n);
  init_graph(m, n, V1);
  return V1;
}

long choose(long a, long b) {
  /* a * (a - 1) * ... * (a - b + 1) / (b * (b - 1) * (b - 2) ... 1) */
  if (2 * b > a) return choose(a, a - b);
  long p = 1;
  long i;
  for (i = 1; i <= b; i++) {
    p *= (a - b + i);
    assert(p % i == 0);
    p /= i;
  }
  return p;
}

int main(int argc, char ** argv) {
  M = (argc > 1 ? atol(argv[1]) : 10);
  N = (argc > 2 ? atol(argv[2]) : 10);
  V1 = make_graph(M, N);
  myth_create_join_many_ex(0, 0, visit_node, V1, 0,
			   0, 0,             sizeof(node), 0,
			   M * N);
  assert(V1[0].r == choose(M + N - 2, M - 1));
  printf("ans = %ld\n", V(0,0).r);
  printf("OK\n");
  return 0;
}
