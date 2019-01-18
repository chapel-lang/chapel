/*
 * treecode.c
 * A treecode implementation to achive best scalability on shared memory machines.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>

#ifndef	PARALLELIZE
#define PARALLELIZE	1
#endif

#ifndef USE_MEMPOOL
#define USE_MEMPOOL 1
#endif

#define	N_CHILD		8

typedef double real_t;
typedef long idx_t;
typedef unsigned long mot_t;

typedef struct vec {
  double x, y, z;  
} vec_t;

typedef struct mol {
  idx_t id; 
  mot_t mid;
  double mass;
  vec_t acc;
  vec_t pos;
  vec_t vel;
} mol_t;

/* tree */
typedef struct node {
  struct node *child[N_CHILD];
  mol_t v_mol;  /* value of node */
  idx_t n_mol;  /* # of mols in tree */
  idx_t molarr_l, molarr_h; /* array range */
  idx_t morton_l, morton_h; /* morton range */
#if USE_MEMPOOL
  int ith, depth;
  idx_t memp_idx;
#endif
} node_t;

/* simulation data structure */
typedef struct sim {  // simulation instance
  unsigned long n_mol;
  int n_step;
  int steps;
  int depth;
  mol_t *mols;
  node_t *tree;
  node_t **memp; /* memory pool, indexed by morton id + depth */
  idx_t memp_len;
  long int t_init, t_treebuild, t_treetraversal, t_treefree;
} sim_t;
sim_t sim_instance;
sim_t *sim = &sim_instance;
  

/* uitilities */
inline int curr_time_micro(void)
{
  struct timeval tp[1];
  struct timezone tzp[1];
  gettimeofday(tp, tzp);
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

inline void * xcmalloc(size_t size)
{
  void *p = malloc(size);
  if (p == NULL) {
    perror("malloc failed");
    abort();
  }
  memset(p, 0x0, size);
  return p;
}

/* tree routines */
inline node_t * memp_get(idx_t idx)
{
  node_t *ptr = sim->memp[idx];
  if (ptr) {
    sim->memp[idx] = NULL;
  } else {
    ptr = xmalloc(sizeof(node_t));
    memset(ptr, 0x0, sizeof(node_t));
    ptr->memp_idx = idx;
  }
  return ptr;
}

inline void memp_put(node_t * ptr)
{
  idx_t idx = ptr->memp_idx;
  if (sim->memp[idx]) {
    printf("warning: memory pool put failed for %ld\n", idx);
  } else {
    sim->memp[idx] = ptr;
  }
}

inline int bin_search(mol_t *mols, mot_t key, 
  idx_t imin, idx_t imax, int low_bound)
{
  idx_t imid;
  
  while (imax >= imin) {
    imid = (imin + imax) / 2;
    if (mols[imid].mid < key)
      imin = imid + 1;
    else if (mols[imid].mid > key)
      imax = imid - 1;
    else
      return imid;
  }
  return low_bound ? imin : imax;
}

// Given ml and mh, find a subrange [il, ih] from original [il, ih]
int select_subarr(mol_t *mols, mot_t ml, mot_t mh, idx_t *il, idx_t *ih)
{
  idx_t _low, _high;

  if (mols[*ih].mid < ml || mols[*il].mid > mh) return 1;
  if ((_low = bin_search(mols, ml, *il, *ih, 1)) == -1) return 1;
  if ((_high = bin_search(mols, mh, *il, *ih, 0)) == -1) return 1;
  if (_low > _high) return 1;
  *il = _low;
  *ih = _high;
  return 0;
}

void tree_build_serial(node_t * node)
{
  idx_t curr_il, curr_ih;
  mot_t curr_ml, curr_mh;
  real_t stride;
  mol_t * mols;
  int i;
  
  mols = sim->mols;
  if (node->n_mol == 1) {
    node->v_mol = mols[node->molarr_l];
  } else if (node->n_mol > 1) {
    stride = (node->morton_h - node->morton_l) / N_CHILD;
    curr_il = node->molarr_l;
    curr_ih = node->molarr_h;
    for (i = 0; i < N_CHILD; i++) {
      curr_ml = node->morton_l + stride * i + i;
      curr_mh = curr_ml + stride;
      if (select_subarr(mols, curr_ml, curr_mh, &curr_il, &curr_ih) == 0) {
        /* subspaces is non-empty */
        node->child[i] = xcmalloc(sizeof(node_t));
        node->child[i]->n_mol = curr_ih - curr_il + 1; /* assert(n_mol>0) */
        node->child[i]->molarr_l = curr_il;
        node->child[i]->molarr_h = curr_ih;
        node->child[i]->morton_l = curr_ml;
        node->child[i]->morton_h = curr_mh;
        curr_il = curr_ih;
        
        tree_build_serial(node->child[i]);
      } else {
        curr_il = node->molarr_l;
      }
      curr_ih = node->molarr_h;
    }
  }
}

void * tree_build_parallel(void *args)
{
  pthread_t ths[N_CHILD];
  idx_t curr_il, curr_ih, base, offset, ith;
  mot_t curr_ml, curr_mh;
  real_t stride;
  mol_t * mols;
  node_t * node;
  int i, n_ths, curr_depth;
  
  n_ths = 0;
  mols = sim->mols;
  node = (node_t *) args;
  if (node->n_mol == 1) {
    node->v_mol = mols[node->molarr_l];
  } else if (node->n_mol > 1) {
    stride = (node->morton_h - node->morton_l) / N_CHILD;
    curr_il = node->molarr_l;
    curr_ih = node->molarr_h;

#if USE_MEMPOOL
    curr_depth = node->depth + 1;
    base = (pow(N_CHILD, curr_depth) - 1) / (N_CHILD - 1);
    offset = node->ith * N_CHILD;
#endif

    for (i = 0; i < N_CHILD; i++) {
      curr_ml = node->morton_l + stride * i + i;
      curr_mh = curr_ml + stride;
      if (select_subarr(mols, curr_ml, curr_mh, &curr_il, &curr_ih) == 0) {
        /* subspaces is non-empty */
#if USE_MEMPOOL
        ith = offset + i;
        node->child[i] = memp_get(base + ith);
        node->child[i]->ith = ith;
        node->child[i]->depth = curr_depth;
#else
        node->child[i] = xcmalloc(sizeof(node_t));
#endif
        node->child[i]->n_mol = curr_ih - curr_il + 1; /* assert(n_mol>0) */
        node->child[i]->molarr_l = curr_il;
        node->child[i]->molarr_h = curr_ih;
        node->child[i]->morton_l = curr_ml;
        node->child[i]->morton_h = curr_mh;
        curr_il = curr_ih;
        pthread_create(&ths[n_ths], NULL, tree_build_parallel, node->child[i]);
        n_ths++;
      } else {
        curr_il = node->molarr_l;
      }
      curr_ih = node->molarr_h;
    }
  }
  for (i = 0; i < n_ths; i++)
    pthread_join(ths[i], NULL);
}

void tree_build(void)
{
  node_t * root;

  sim->t_treebuild = curr_time_micro();
#if USE_MEMPOOL
  root = memp_get(0);
  root->ith = 0;
  root->depth = 0;
  root->memp_idx = 0;
#else
  root = (node_t *) xcmalloc(sizeof(node_t));
#endif

  root->n_mol = sim->n_mol;
  root->molarr_l = 0;
  root->molarr_h = sim->n_mol - 1;
  root->morton_l = sim->mols[root->molarr_l].mid;
  root->morton_h = sim->mols[root->molarr_h].mid;
  sim->tree = root;

#if PARALLELIZE
  tree_build_parallel(sim->tree);
#else
  tree_build_serial(sim->tree);
#endif

  sim->t_treebuild = curr_time_micro() - sim->t_treebuild;
}

void tree_free_serial(node_t * node)
{
  int i;

  if (node == NULL) return;

  for (i=0; i < N_CHILD; i++)
    tree_free_serial(node->child[i]);

  free(node);
}

void * tree_free_parallel(void * args)
{
  pthread_t ths[N_CHILD];
  node_t *p = (node_t *) args;
  int i;

  if (p == NULL) return;

  for (i = 0; i < N_CHILD; i++) {
    if (i < N_CHILD - 1)
      pthread_create(&ths[i], NULL, tree_free_parallel, p->child[i]);
    else
      tree_free_parallel(p->child[i]);
  }

  for (i = 0; i < N_CHILD - 1; i++)
    pthread_join(ths[i], NULL);

#if USE_MEMPOOL
  memp_put(p);
#else
  free(p);
#endif
}

void tree_free(void)
{

  sim->t_treefree = curr_time_micro();

#if PARALLELIZE
  tree_free_parallel(sim->tree);
#else
  tree_free_serial(sim->tree);
#endif
  
  sim->t_treefree = curr_time_micro() - sim->t_treefree;
}

void tree_check_walk(node_t *node, int *n, int depth) {
  int i;
  
  if (node == NULL) return;
  
  (*n)++;
  for (i = 0; i < N_CHILD; i++) {
    tree_check_walk(node->child[i], n, depth + 1);
  }
}

void tree_valid_check(void)
{
  int n_total = 0;

  sim->t_treetraversal = curr_time_micro();
  tree_check_walk(sim->tree, &n_total, 0);
  sim->t_treetraversal = curr_time_micro() - sim->t_treetraversal;
  if (n_total != (pow(N_CHILD, sim->depth + 1) - 1) / (N_CHILD - 1))
    printf(" valid check failed: walked %d nodes\n", n_total);
}

/* simulation routines */
void init(int argc, char *argv[])
{
  idx_t i;
  
  curr_time_micro();  /* dummy call to make first timing accurate, Darwin only? */
  
  if (argc < 3) {
    printf("usage: %s DEPTH STEPS\n", argv[0]);
    exit(0);
  }
  sim->depth = atoi(argv[1]);
  sim->n_step = atoi(argv[2]);
  sim->n_mol = pow(N_CHILD, sim->depth); 
  sim->steps = 0;
  sim->t_treebuild = 0;
  sim->mols = xmalloc(sizeof(mol_t) * sim->n_mol);
  for (i = 0; i < sim->n_mol; i++) {
    sim->mols[i].id = i;
    sim->mols[i].mid = i;
  }
#if USE_MEMPOOL
  sim->memp_len = (pow(N_CHILD, sim->depth + 1) - 1) / (N_CHILD - 1);
  sim->memp = xmalloc(sizeof(node_t *) * sim->memp_len);
  memset(sim->memp, 0x0, sizeof(node_t *) * sim->memp_len);
  printf("N=%ld, STEPS=%d, MEMPOOL=%ld\n", 
    sim->n_mol, sim->n_step, sim->memp_len);
#else
  printf("N=%ld, STEPS=%d\n", sim->n_mol, sim->n_step);
#endif
  
}

void finalize(void)
{
  idx_t i;
  free(sim->mols);
#if USE_MEMPOOL
  for (i = 0; i < sim->memp_len; i++)
    free(sim->memp[i]);
  free(sim->memp);
#endif
}

void status(void)
{
  fprintf(stdout, "[%03d] TreeBuild: %ld, TreeTraveral: %ld, TreeFree: %ld\n", sim->steps,
    sim->t_treebuild, sim->t_treetraversal, sim->t_treefree);
}

int main(int argc, char *argv[])
{
  init(argc, argv);
  
  while (sim->steps < sim->n_step) {
    tree_build();
    tree_valid_check();
    tree_free();
    status();
    sim->steps++;
  }

  finalize();

  return 0;
}
