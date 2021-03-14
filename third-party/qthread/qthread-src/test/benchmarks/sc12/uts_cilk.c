/******************************************************
* Unbalanced Tree Search v2.1                        *
* Based on the implementation available at           *
*     http://sourceforge.net/projects/uts-benchmark  *
******************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* for INT_MAX */
#include <math.h>   /* for floor, log, sin */
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>
#include <pthread.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>

#define SILENT_ARGPARSING
#include "argparsing.h"
#include "log.h"

#define BRG_RNG // Select RNG
#include "../../utils/rng/rng.h"

#define PRINT_STATS 1

#define MAXNUMCHILDREN 100

static size_t nodecount;

typedef enum {
    BIN = 0,
    GEO,
    HYBRID,
    BALANCED
} tree_t;
static char *type_names[] = {
    "Binomial",
    "Geometric",
    "Hybrid",
    "Balanced"
};

typedef enum {
    LINEAR = 0,
    EXPDEC,
    CYCLIC,
    FIXED
} shape_t;
static char *shape_names[] = {
    "Linear decrease",
    "Exponential decrease",
    "Cyclic",
    "Fixed branching factor"
};

typedef struct {
    int            height; // Depth of node in the tree
    struct state_t state;  // Local RNG state
    int            num_children;
} node_t;

// Default values
static tree_t  tree_type     = GEO;
static double  bf_0          = 4.0;
static int     root_seed     = 0;
static int     num_samples   = 1;
static int     tree_depth    = 6;
static shape_t shape_fn      = LINEAR;
static int     non_leaf_bf   = 4;
static double  non_leaf_prob = 15.0 / 64.0;
static double  shift_depth   = 0.5;

// Tree metrics
static uint64_t tree_height = 0;
static uint64_t num_leaves  = 0;

static double normalize(int n)
{
    if (n < 0) {
        printf("*** toProb: rand n = %d out of range\n", n);
    }

    return ((n < 0) ? 0.0 : ((double)n) / (double)INT_MAX);
}

static int calc_num_children_bin(node_t *parent)
{
    int    v = rng_rand(parent->state.state);
    double d = normalize(v);

    return (d < non_leaf_prob) ? non_leaf_bf : 0;
}

static int calc_num_children(node_t *parent)
{
    int num_children = 0;

    if (parent->height == 0) { num_children = (int)floor(bf_0); } else { num_children = calc_num_children_bin(parent); }

    if (parent->height == 0) {
        int root_bf = (int)ceil(bf_0);
        if (num_children > root_bf) {
            printf("*** Number of children truncated from %d to %d\n",
                   num_children, root_bf);
            num_children = root_bf;
        }
    } else   {
        if (num_children > MAXNUMCHILDREN) {
            printf("*** Number of children truncated from %d to %d\n",
                   num_children, MAXNUMCHILDREN);
            num_children = MAXNUMCHILDREN;
        }
    }

    return num_children;
}

// Notes:
// -    Each task receives distinct copy of parent
// -    Copy of child is shallow, be careful with `state` member
static long visit(node_t parent)
{
    node_t    child;
    uint64_t *child_descendants = calloc(sizeof(long), parent.num_children);

    CILK_C_REDUCER_OPADD(num_descendants, ulong, 0);
    uint64_t tmp;

    // Spawn children, if any
    for (int i = 0; i < parent.num_children; i++) {
        child.height = parent.height + 1;

        for (int j = 0; j < num_samples; j++) {
            rng_spawn(parent.state.state, child.state.state, i);
        }

        child.num_children = calc_num_children(&child);

        child_descendants[i] = _Cilk_spawn visit(child);
    }

    _Cilk_sync;

    CILK_C_REGISTER_REDUCER(num_descendants);

    _Cilk_for(int i = 0; i < parent.num_children; i++) {
        REDUCER_VIEW(num_descendants) += child_descendants[i];
    }

    tmp = 1 + REDUCER_VIEW(num_descendants);

    CILK_C_UNREGISTER_REDUCER(num_descendants);

    return tmp;
}

#ifdef PRINT_STATS
static void print_stats(void)
{
    LOG_UTS_PARAMS_YAML()

    fflush(stdout);
}

#else /* ifdef PRINT_STATS */
static void print_banner(void)
{
    printf("UTS - Unbalanced Tree Search 2.1 (C/Qthreads)\n");
    printf("Tree type:%3d (%s)\n", tree_type, type_names[tree_type]);
    printf("Tree shape parameters:\n");
    printf("  root branching factor b_0 = %.1f, root seed = %d\n",
           bf_0, root_seed);

    if ((tree_type == GEO) || (tree_type == HYBRID)) {
        printf("  GEO parameters: gen_mx = %d, shape function = %d (%s)\n",
               tree_depth, shape_fn, shape_names[shape_fn]);
    }

    if ((tree_type == BIN) || (tree_type == HYBRID)) {
        double q  = non_leaf_prob;
        int    m  = non_leaf_bf;
        double es = (1.0 / (1.0 - q * m));
        printf("  BIN parameters: q = %f, m = %d, E(n) = %f, E(s) = %.2f\n",
               q, m, q * m, es);
    }

    if (tree_type == HYBRID) {
        printf("  HYBRID: GEO from root to depth %d, then BIN\n",
               (int)ceil(shift_depth * tree_depth));
    }

    if (tree_type == BALANCED) {
        printf("  BALANCED parameters: gen_mx = %d\n", tree_depth);
        printf("    Expected size: %llu nodes, %llu leaves\n",
               (unsigned long long)((pow(bf_0, tree_depth + 1) - 1.0) / (bf_0 - 1.0)),
               (unsigned long long)pow(bf_0, tree_depth));
    }

    printf("Random number generator: ");
    printf("SHA-1 (state size = %ldB)\n", sizeof(struct state_t));
    printf("Compute granularity: %d\n", num_samples);
    printf("Execution strategy:\n");
    printf("  Workers:   %d\n", __cilkrts_get_nworkers());

    printf("\n");

    fflush(stdout);
}

#endif /* ifdef PRINT_STATS */

int main(int   argc,
         char *argv[])
{
    uint64_t total_num_nodes = 0;
    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    {
        unsigned long tmp = 0;
        NUMARG(tmp, "UTS_TREE_TYPE");
        tree_type = (tree_t)tmp;
    }
    DBLARG(bf_0, "UTS_BF_0");
    NUMARG(root_seed, "UTS_ROOT_SEED");
    {
        unsigned long tmp = 0;
        NUMARG(tmp, "UTS_SHAPE_FN");
        shape_fn = (shape_t)tmp;
    }
    NUMARG(tree_depth, "UTS_TREE_DEPTH");
    DBLARG(non_leaf_prob, "UTS_NON_LEAF_PROB");
    NUMARG(non_leaf_bf, "UTS_NON_LEAF_NUM");
    NUMARG(shift_depth, "UTS_SHIFT_DEPTH");
    NUMARG(num_samples, "UTS_NUM_SAMPLES");

#ifdef PRINT_STATS
    print_stats();
#else
    print_banner();
#endif

    timer = qtimer_create();
    qtimer_start(timer);

    node_t root;
    root.height = 0;
    rng_init(root.state.state, root_seed);
    root.num_children = calc_num_children(&root);

    nodecount = 1;
    long retval;
    {
        retval = _Cilk_spawn visit(root);

        _Cilk_sync;
    }

    total_num_nodes = retval;

    qtimer_stop(timer);

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

#ifdef PRINT_STATS
    LOG_UTS_RESULTS_YAML(total_num_nodes, total_time)
    LOG_ENV_CILK_YAML()
#else
    printf("Tree size = %lu, tree depth = %d, num leaves = %llu (%.2f%%)\n",
           (unsigned long)total_num_nodes,
           (int)tree_height,
           (unsigned long long)num_leaves,
           num_leaves / (float)total_num_nodes * 100.0);
    printf("Wallclock time = %.3f sec, performance = %.0f "
           "nodes/sec (%.0f nodes/sec per PE)\n\n",
           total_time,
           total_num_nodes / total_time,
           total_num_nodes / total_time / __cilkrts_get_nworkers());
#endif /* ifdef PRINT_STATS */

    return 0;
}

/* vim:set expandtab */
