#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#ifndef PARALLELIZE
#define PARALLELIZE 1
#endif

#ifndef USE_MEMPOOL
#define USE_MEMPOOL 1
#endif
#ifndef USE_CONTMEM /* use continuous memory */
#define USE_CONTMEM 0
#endif

#ifndef BUILD_TOPDOWN
#define BUILD_TOPDOWN 0
#endif

#define N_CHILD  8

int G_MAX_DEPTH = 7;
int G_INC_DEPTH = 0;
int G_ITERATION = 11;
int G_TRACEITER = -1;
int G_ITER = 0;

typedef struct node {
  int depth;
  struct node* child[N_CHILD];
  int ith, memp_idx;
} node_t; 
node_t **G_MEMPOOL = NULL;
node_t *G_CONTMEM = NULL;

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

inline void eat_cpu(int n)
{
  struct timespec req, rem;
  req.tv_sec = 0;
  req.tv_nsec = n; /* millisecond */
  nanosleep(&req, &rem);
}

inline node_t * memp_get(int idx)
{
  node_t *ptr = G_MEMPOOL[idx];
  if (ptr) {
    G_MEMPOOL[idx] = NULL;
  } else {
    ptr = xmalloc(sizeof(node_t));
    memset(ptr, 0x0, sizeof(node_t));
    ptr->memp_idx = idx;
  }
  return ptr;
}

inline void memp_put(node_t * ptr)
{
  int idx = ptr->memp_idx;
  if (G_MEMPOOL[idx]) {
    printf("warning: memory pool put failed for %d\n", idx);
  } else {
    G_MEMPOOL[idx] = ptr;
  }
}

inline node_t * new_node(void) {
  node_t *n = (node_t *) xmalloc(sizeof(struct node));
  memset(n, 0x0, sizeof(node_t));
  return n;
}

void tree_build_serial_topdown(node_t * node)
{
  int i, child_depth, base, offset, ith;
  

  if (node->depth >= G_MAX_DEPTH) return;

  child_depth = node->depth + 1;
#if USE_MEMPOOL
  base = quick_base(child_depth);
  offset = node->ith * N_CHILD;
#endif

  for (i = 0; i < N_CHILD; i++) {
#if USE_MEMPOOL
    ith = offset + i;
    node->child[i] = memp_get(base + ith);
    node->child[i]->ith = ith;
#else
    node->child[i] = new_node();
#endif

    node->child[i]->depth = child_depth;
    if (child_depth < G_MAX_DEPTH)
      tree_build_serial_topdown(node->child[i]);
  }
}

void tree_build_serial_downtop(node_t * node)
{
  int i, child_depth, base, offset, ith;
  node_t nodes[N_CHILD];


  if (node->depth >= G_MAX_DEPTH) return;
  
  child_depth = node->depth + 1;
  memset(nodes, 0x0, sizeof(node_t) * N_CHILD);

#if USE_MEMPOOL
  base = quick_base(child_depth);
  offset = node->ith * N_CHILD;
#endif

  for (i = 0; i < N_CHILD; i++) {
#if USE_MEMPOOL
    ith = offset + i;
    nodes[i].ith = ith;
    nodes[i].memp_idx = base + ith;
#endif
    
    nodes[i].depth = child_depth;

    if (child_depth < G_MAX_DEPTH)
      tree_build_serial_downtop(&nodes[i]);

#if USE_MEMPOOL
    node->child[i] = memp_get(nodes[i].memp_idx);
#else
    node->child[i] = new_node();
#endif

    *(node->child[i]) = nodes[i];
  }
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

void * tree_build_parallel_topdown(void * args)
{
  pthread_t ths[N_CHILD];
  node_t *node = (node_t *) args;
  int i, child_depth, base, offset, ith;
  int t0, t1, t2, t3;
  
  if (node->depth >= G_MAX_DEPTH) return (void *) 0;

  child_depth = node->depth + 1;

#if USE_MEMPOOL
  base = quick_base(child_depth);
  offset = node->ith * N_CHILD;
#endif
  
  //eat_cpu(1);
  
  //t0 = curr_time_micro();
  for (i = 0; i < N_CHILD; i++) {
#if USE_MEMPOOL
    ith = offset + i;
    node->child[i] = memp_get(base + ith);
    node->child[i]->ith = ith;
    node->child[i]->depth = child_depth;
#else
    node->child[i] = new_node(child_depth); 
#endif
  }

  if (child_depth >= G_MAX_DEPTH) return (void *) 0;

  //t1 = curr_time_micro();

  for (i = 0; i < N_CHILD; i++) {
    if (i < N_CHILD - 1) {
      pthread_create(&ths[i], NULL, tree_build_parallel_topdown, node->child[i]);
    } else {
      tree_build_parallel_topdown(node->child[i]);
    }
  }

  //t2 = curr_time_micro();
  
  for (i = 0; i < N_CHILD - 1; i++)
    pthread_join(ths[i], NULL);
  
  //t3 = curr_time_micro();
  
  //if (G_ITER > 0)
  //  fprintf(stderr, "tree_build %d %d %d\n", t1-t0, t2-t1, t3-t2);

  return (void *) 0;
}

void * tree_build_parallel_downtop(void * args)
{
  pthread_t ths[N_CHILD];
  node_t *node = (node_t *) args;
  node_t nodes[N_CHILD];
  int i, child_depth, base, offset, ith;
  int t0, t1, t2, t3;

  
  if (node->depth >= G_MAX_DEPTH) return;

  child_depth = node->depth + 1;
  memset(nodes, 0x0, sizeof(node_t) * N_CHILD);

#if USE_MEMPOOL
  base = quick_base(child_depth);
  offset = node->ith * N_CHILD;
#endif
    
  //eat_cpu(1);
  
  for (i = 0; i < N_CHILD; i++) {
#if USE_MEMPOOL
    ith = offset + i;
    nodes[i].ith = ith;
    nodes[i].memp_idx = base + ith;
#endif

    nodes[i].depth = child_depth;
  }

  if (child_depth < G_MAX_DEPTH) {
    for (i = 0; i < N_CHILD; i++) {
      if (i < N_CHILD - 1) {
        pthread_create(&ths[i], NULL, tree_build_parallel_downtop, &nodes[i]);
      } else {
        tree_build_parallel_downtop(&nodes[i]);
      }
    }
    for (i = 0; i < N_CHILD - 1; i++)
      pthread_join(ths[i], NULL);
  }

  for (i = 0; i < N_CHILD; i++) {
#if USE_MEMPOOL
    node->child[i] = memp_get(nodes[i].memp_idx);
#else
    node->child[i] = new_node();
#endif

    *(node->child[i]) = nodes[i];
  }
  
  return (void *) 0;
}

void tree_build(node_t * tree)
{
#if PARALLELIZE

#if BUILD_TOPDOWN
  tree_build_parallel_topdown(tree);
#else 
  tree_build_parallel_downtop(tree);
#endif /* BUILD_TOPDOWN */

#else /* PARALLELIZE */

#if BUILD_TOPDOWN
  tree_build_serial_topdown(tree);
#else
  tree_build_serial_downtop(tree);
#endif /* BUILD_TOPDOWN */

#endif /* PARALLELIZE */
}

void tree_traversal_serial(node_t *node, int *n)
{
  int i;
  
  if (node == NULL) return;

  (*n)++;
  for (i = 0; i < N_CHILD; i++)
    tree_traversal_serial(node->child[i], n);
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
  int i;

  if (p->node == NULL) return (void *) 0;
  
  p->n = 1;
  if (p->node->child[0]) {
    for (i = 0; i < N_CHILD; i++) {
      dat[i].node = p->node->child[i];
      if (i < N_CHILD - 1)
        pthread_create(&ths[i], NULL, tree_traversal_parallel, &dat[i]);
      else
        tree_traversal_parallel(&dat[i]);
    }
    
    for (i = 0; i < N_CHILD; i++) {
      if (i < N_CHILD - 1)
        pthread_join(ths[i], NULL);
      p->n += dat[i].n;
    }
  }
}

void tree_traversal(node_t *node, int *n)
{
#if PARALLELIZE
  struct count_nodes dat;
  dat.node = node;
  dat.n = 0;
  tree_traversal_parallel(&dat);
  *n = dat.n;
#else
  tree_traversal_serial(node, n);
#endif
}

void * tree_free_parallel(void * args)
{
  pthread_t ths[N_CHILD];
  struct node * p = (struct node *) args;
  int i;

  if (p == NULL) return (void *) 0;
  
  if (p->child[0]) {  /* CRITICAL SPEEDUP */
    for (i = 0; i < N_CHILD; i++) 
      pthread_create(&ths[i], NULL, tree_free_parallel, p->child[i]);
    
    for (i = 0; i < N_CHILD; i++) {
      pthread_join(ths[i], NULL);
    }
  }
 
  if (p->depth > G_INC_DEPTH) {
#if USE_MEMPOOL
    memp_put(p);
#else
    free(p);
#endif
  }
}

void tree_free_serial(node_t *node)
{
  int i;

  if (node == NULL) return;

  for (i = 0; i < N_CHILD; i++)
    tree_free_serial(node->child[i]);
  free(node);
}

void tree_free(node_t *tree)
{
#if PARALLELIZE
  tree_free_parallel(tree);
#else
  tree_free_serial(tree);
#endif
}

int main(int argc, char *argv[])
{
  struct node *root;
  int total, n, inc;
  int i, t0, t1, t2, t3;

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

  if (G_INC_DEPTH >= G_MAX_DEPTH)
    G_INC_DEPTH = G_MAX_DEPTH - 1;

  total = (pow(N_CHILD, G_MAX_DEPTH + 1) - 1) / (N_CHILD - 1);
#if USE_MEMPOOL
  G_MEMPOOL = xmalloc(sizeof(node_t *) * total);
  memset(G_MEMPOOL, 0x0, sizeof(node_t *) * total);
#if USE_CONTMEM
  G_CONTMEM = xmalloc(sizeof(node_t) * total);
  memset(G_CONTMEM, 0x0, sizeof(node_t) * total);
  for (i = 0; i < total; i++)
    G_MEMPOOL[i] = &(G_CONTMEM[i]);
#endif /* USE_CONTMEM */
#endif /* USE_MEMPOOL */

  printf("Depth: %d (inc %d), Iter: %d (mtrace the %dth), Nodes: %d\n", 
    G_MAX_DEPTH, G_INC_DEPTH, G_ITERATION, G_TRACEITER, total);
  
  inc = G_INC_DEPTH;
  G_INC_DEPTH = 0;
  for (G_ITER = 0; G_ITER < G_ITERATION; G_ITER++) {
    if (G_INC_DEPTH == 0) {
#if USE_MEMPOOL
      root = memp_get(0);
      root->ith = 0;
      root->memp_idx = 0;
#else
      root = new_node(0);
#endif
      root->depth = 0;
    }
    
    t0 = curr_time_micro();
    if (G_ITER == G_TRACEITER) mtrace();
    tree_build(root);
    if (G_ITER == G_TRACEITER) muntrace();
    t1 = curr_time_micro();

    n = 0;
    tree_traversal(root, &n);
    if (n != total)
      printf("Tree check failed! (%d vs. %d)\n", total, n);
    
    G_INC_DEPTH = inc;

    t2 = curr_time_micro();
    tree_free(root);
    t3 = curr_time_micro();
    printf("[%02d] Build: %d Traversal: %d Free: %d\n", G_ITER, t1-t0, t2-t1, t3-t2);
  }

#if USE_MEMPOOL
#if USE_CONTMEM
  free(G_CONTMEM);
#else
  for (i = 0; i < total; i++)
    free(G_MEMPOOL[i]);;
#endif /* USE_CONTMEM */
  free(G_MEMPOOL);
#endif /* USE_MEMPOOL */
  
  return 0;
}
