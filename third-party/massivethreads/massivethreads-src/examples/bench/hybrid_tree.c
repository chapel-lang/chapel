#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#ifndef PARALLELIZE
#define PARALLELIZE 1
#endif

#define N_CHILD  8

int G_MAX_DEPTH = 7;
int G_ITERATION = 11;
int G_TRACEITER = -1;
int G_ITER = 0;

typedef struct node {
  int dep;      /* depth in tree */
  int off;      /* offset at the same depth */
  int idx;     /* index of memory array, -1 indicates using pointer */
  int nchild;   /* number of children */
} node_t;
node_t *G_NODES_ARR;

inline int curr_time_micro(void)
{
  struct timeval tp[1];
  struct timezone tzp[1];
  gettimeofday (tp, tzp);
  return tp->tv_sec * 1000000 + tp->tv_usec;
}

inline void * xmalloc(size_t size)
{
  void *p = malloc(size);
  if (p == NULL) {
    perror("malloc failed");
    abort();
  }
  return p;
}

inline int quick_base(int depth)
{
  switch (depth) {
    case 0: return 0;
    case 1: return 1;
    case 2: return 9;
    case 3: return 73;
    case 4: return 585;
    case 5: return 4681;
    case 6: return 37449;
    case 7: return 299593;
    case 8: return 2396745;
    case 9: return 19173961;
    case 10: return 153391689;
    default: return (pow(N_CHILD, depth) - 1) / (N_CHILD - 1);
  }
}

void tree_build_serial(node_t *node)
{
  int child_dep, base, offset, off, idx, i;
  node_t *child;

  if (node->dep >= G_MAX_DEPTH) return;

  child_dep = node->dep + 1;
  base = quick_base(child_dep);
  offset = node->off * N_CHILD;
  node->nchild = N_CHILD;
  
  for (i = 0; i < N_CHILD; i++) {
    off = offset + i;
    idx = base + off;
    child = G_NODES_ARR + idx;
    child->dep = child_dep;
    child->off = off;
    child->idx = idx;
    child->nchild = 0;
    if (child_dep < G_MAX_DEPTH) {
      tree_build_serial(child);
    }
  }
}

void * tree_build_parallel(void *args)
{
  pthread_t ths[N_CHILD];
  node_t *node = (node_t *) args;
  int child_dep, base, offset, off, idx, i;
  node_t *child;

  if (node->dep >= G_MAX_DEPTH) return;

  child_dep = node->dep + 1;
  base = quick_base(child_dep);
  offset = node->off * N_CHILD;
  node->nchild = N_CHILD;
  
  for (i = 0; i < N_CHILD; i++) {
    off = offset + i;
    idx = base + off;
    child = G_NODES_ARR + idx;
    child->dep = child_dep;
    child->off = off;
    child->idx = idx;
    child->nchild = 0;
    if (child_dep < G_MAX_DEPTH) {
      if (i < N_CHILD - 1)
        pthread_create(&ths[i], NULL, tree_build_parallel, child);
      else
        tree_build_serial(child);
    }
  }

  if (child_dep < G_MAX_DEPTH) {
    for (i = 0; i < N_CHILD - 1; i++)
      pthread_join(ths[i], NULL);
  }
}

void tree_build(node_t *root)
{
#if PARALLELIZE
  tree_build_parallel(root);
#else
  tree_build_serial(root);
#endif
}

void tree_traversal_serial(node_t *node, int *n)
{
  int offset, i;

  (*n)++;
  offset = quick_base(node->dep + 1) + node->off * N_CHILD;
  for (i = 0; i < node->nchild; i++)
    tree_traversal_serial(G_NODES_ARR + offset + i, n);
}

struct count_nodes {
  node_t *node;
  int n;
};

void * tree_traversal_parallel(void *args)
{
  pthread_t ths[N_CHILD];
  struct count_nodes dat[N_CHILD];
  struct count_nodes * p = (struct count_nodes *) args;
  int offset, i;

  p->n = 1;
  offset = quick_base(p->node->dep + 1) + p->node->off * N_CHILD;
  for (i = 0; i < p->node->nchild; i++) {
    dat[i].node = G_NODES_ARR + offset + i;
    if (i < p->node->nchild - 1) {
      pthread_create(&ths[i], NULL, tree_traversal_parallel, &dat[i]); 
    } else
      tree_traversal_parallel(&dat[i]);
  }

  for (i = 0; i < p->node->nchild; i++) {
    if (i < p->node->nchild - 1)
      pthread_join(ths[i], NULL);
    p->n += dat[i].n;
  }
}

void tree_traversal(node_t *root, int *n)
{
#if PARALLELIZE
  struct count_nodes dat;
  dat.node = root;
  dat.n = 0;
  tree_traversal_parallel(&dat);
  *n = dat.n;
#else
  tree_traversal_serial(root, n);
#endif
}

void tree_free(void)
{
  int total = (pow(N_CHILD, G_MAX_DEPTH + 1) - 1) / (N_CHILD - 1);
  memset(G_NODES_ARR, 0x0, sizeof(node_t) * total);
}

int main(int argc, char *argv[])
{
  int total;
  int n, i, t0, t1, t2, t3;
  node_t root;

  if (argc < 2) {
    printf("Arguments not sufficient, use default values\n");
  } else {
    G_MAX_DEPTH = atoi(argv[1]);
    if (argc >= 3)
      G_ITERATION = atoi(argv[2]);
    if (argc >= 4)
      G_TRACEITER = atoi(argv[3]);
    else
      G_TRACEITER = 0.8 * G_ITERATION;
  }
  
  total = (pow(N_CHILD, G_MAX_DEPTH + 1) - 1) / (N_CHILD - 1);
  G_NODES_ARR = xmalloc(sizeof(node_t) * total);
  memset(G_NODES_ARR, 0x0, sizeof(node_t) * total);
  
  printf("Depth: %d, Iter: %d (mtrace the %dth), Nodes: %d\n", 
    G_MAX_DEPTH, G_ITERATION, G_TRACEITER, total);
  
  root.dep = 0;
  root.off = 0;
  root.idx = 0;
  root.nchild = 0;
  for (G_ITER = 0; G_ITER < G_ITERATION; G_ITER++) {
    t0 = curr_time_micro();
    if (G_ITER == G_TRACEITER) mtrace();
    tree_build(&root);
    if (G_ITER == G_TRACEITER) muntrace();
    t1 = curr_time_micro();
  
    /*
    for (i = 0; i < total; i++) {
      node_t *p = G_NODES_ARR + i;
      printf("%d: %d %d %d %d\n", i, p->dep, p->off, p->idx, p->nchild);
    }
    */
    
    n = 0;
    tree_traversal(&root, &n);
    if (n != total)
      printf("Tree check failed! (%d vs. %d)\n", total, n);
    
    t2 = curr_time_micro();
    tree_free();
    t3 = curr_time_micro();
    printf("[%02d] Build: %d Traversal: %d Free: %d\n", G_ITER, t1-t0, t2-t1, t3-t2);
  }

  return 0;
}
