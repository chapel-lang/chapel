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
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include <qthread/spr.h>
#include <qthread/multinode.h>
#include "argparsing.h"

#define BRG_RNG // Select RNG
#include "../utils/rng/rng.h"

#define COLLECT_STATS 1

#define PRINT_STATS 1

#define MAXNUMCHILDREN 100

static int    num_locales;
static int    here;
static int    talk_to_self = 0;
static double perc_remote  = 0.0;

#ifdef COLLECT_STATS
static aligned_t *comm_bins    = NULL;
static aligned_t  num_messages = 0;

static aligned_t pong_comm(void *args_)
{
    aligned_t *remote_comm_bins = (aligned_t *)args_;

    aligned_t num_out = 0;

    iprintf("%d ", here);
    for (int i = 0; i < num_locales; i++) {
        num_out += remote_comm_bins[i];
        iprintf("%4lu ", (unsigned long)remote_comm_bins[i]);
    }
    iprintf("\n");

    return num_out;
}

static aligned_t ping_comm(void *args_)
{
    return pong_comm(comm_bins);
}

#endif /* ifdef COLLECT_STATS */

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
        iprintf("*** toProb: rand n = %d out of range\n", n);
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
            iprintf("*** Number of children truncated from %d to %d\n",
                    num_children, root_bf);
            num_children = root_bf;
        }
    } else {
        if (num_children > MAXNUMCHILDREN) {
            iprintf("*** Number of children truncated from %d to %d\n",
                    num_children, MAXNUMCHILDREN);
            num_children = MAXNUMCHILDREN;
        }
    }

    return num_children;
}

#define BIG_STACKS
// Notes:
// -    Each task receives distinct copy of parent
// -    Copy of child is shallow, be careful with `state` member
static aligned_t visit(void *args_)
{
    node_t  *parent          = (node_t *)args_;
    int      parent_height   = parent->height;
    int      num_children    = parent->num_children;
    uint64_t num_descendants = 1;
    node_t   child;

#ifdef BIG_STACKS
    aligned_t rets[num_children];
#else
    aligned_t *rets;

    if (num_children > 0) {
        rets = malloc(sizeof(aligned_t) * num_children);
    }
#endif

    // Spawn children, if any
    for (int i = 0; i < num_children; i++) {
        rets[i] = 0;
    }
    child.height = parent_height + 1;
    for (int i = 0; i < num_children; i++) {
        for (int j = 0; j < num_samples; j++) {
            rng_spawn(parent->state.state, child.state.state, i);
        }

        child.num_children = calc_num_children(&child);

        int const r = rng_rand(child.state.state);

        int target = (r <= 0x7fffffff * perc_remote) ? r % num_locales : here;

        if (talk_to_self || (target != here)) {
            // Spawn `visit` task to a random locale
            iprintf("%03d : spawning visit on locale %03d\n", here, target);
#ifdef COLLECT_STATS
            qthread_incr(&(comm_bins[target]), 1);
#endif

            qthread_fork_remote(visit, &child, &rets[i],
                                target,
                                sizeof(node_t));
        } else {
            qthread_fork_copyargs(visit, &child, sizeof(node_t), &rets[i]);
        }
    }

    // Wait for children to finish up, accumulate descendants counts
    for (int i = 0; i < num_children; i++) {
        qthread_readFF(NULL, &rets[i]);
        num_descendants += rets[i];
    }

#ifndef BIG_STACKS
    if (num_children > 0) {
        free(rets);
    }
#endif
    return num_descendants;
}

#ifdef PRINT_STATS
static void print_stats(void)
{
    iprintf("tree-type %d\ntree-type-name %s\n",
            tree_type, type_names[tree_type]);
    iprintf("root-bf %.1f\nroot-seed %d\n",
            bf_0, root_seed);

    if ((tree_type == GEO) || (tree_type == HYBRID)) {
        iprintf("gen_mx %d\nshape-fn %d\nshape-fn-name %s\n",
                tree_depth, shape_fn, shape_names[shape_fn]);
    }

    if ((tree_type == BIN) || (tree_type == HYBRID)) {
        double q  = non_leaf_prob;
        int    m  = non_leaf_bf;
        double es = (1.0 / (1.0 - q * m));
        iprintf("q %f\nm %d\nE(n) %f\nE(s) %.2f\n",
                q, m, q * m, es);
    }

    if (tree_type == HYBRID) {
        iprintf("root-to-depth %d\n",
                (int)ceil(shift_depth * tree_depth));
    }

    if (tree_type == BALANCED) {
        iprintf("gen_mx %d\n", tree_depth);
        iprintf("expected-num-nodes %llu\nexpected-num-leaves %llu\n",
                (unsigned long long)((pow(bf_0, tree_depth + 1) - 1.0) / (bf_0 - 1.0)),
                (unsigned long long)pow(bf_0, tree_depth));
    }

    iprintf("compute-granularity %d\n", num_samples);
    iprintf("num-sheps %d\n", qthread_num_shepherds());
    iprintf("num-workers %d\n", qthread_num_workers());

    iprintf("\n");

    fflush(stdout);
}

#else /* ifdef PRINT_STATS */
static void print_banner(void)
{
    iprintf("UTS - Unbalanced Tree Search 2.1 (C/Qthreads)\n");
    iprintf("Tree type:%3d (%s)\n", tree_type, type_names[tree_type]);
    iprintf("Tree shape parameters:\n");
    iprintf("  root branching factor b_0 = %.1f, root seed = %d\n",
            bf_0, root_seed);

    if ((tree_type == GEO) || (tree_type == HYBRID)) {
        iprintf("  GEO parameters: gen_mx = %d, shape function = %d (%s)\n",
                tree_depth, shape_fn, shape_names[shape_fn]);
    }

    if ((tree_type == BIN) || (tree_type == HYBRID)) {
        double q  = non_leaf_prob;
        int    m  = non_leaf_bf;
        double es = (1.0 / (1.0 - q * m));
        iprintf("  BIN parameters: q = %f, m = %d, E(n) = %f, E(s) = %.2f\n",
                q, m, q * m, es);
    }

    if (tree_type == HYBRID) {
        iprintf("  HYBRID: GEO from root to depth %d, then BIN\n",
                (int)ceil(shift_depth * tree_depth));
    }

    if (tree_type == BALANCED) {
        iprintf("  BALANCED parameters: gen_mx = %d\n", tree_depth);
        iprintf("    Expected size: %llu nodes, %llu leaves\n",
                (unsigned long long)((pow(bf_0, tree_depth + 1) - 1.0) / (bf_0 - 1.0)),
                (unsigned long long)pow(bf_0, tree_depth));
    }

    iprintf("Random number generator: ");
    iprintf("SHA-1 (state size = %ldB)\n", sizeof(struct state_t));
    iprintf("Compute granularity: %d\n", num_samples);
    iprintf("Execution strategy:\n");
    iprintf("  Shepherds: %d\n", qthread_num_shepherds());
    iprintf("  Workers:   %d\n", qthread_num_workers());

    iprintf("\n");

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
        unsigned int tmp = (unsigned int)tree_type;
        NUMARG(tmp, "UTS_TREE_TYPE");
        if (tmp <= BALANCED) {
            tree_type = (tree_t)tmp;
        } else {
            fprintf(stderr, "invalid tree type\n");
            return EXIT_FAILURE;
        }
        tmp = (unsigned int)shape_fn;
        NUMARG(tmp, "UTS_SHAPE_FN");
        if (tmp <= FIXED) {
            shape_fn = (shape_t)tmp;
        } else {
            fprintf(stderr, "invalid shape function\n");
            return EXIT_FAILURE;
        }
    }
    DBLARG(bf_0, "UTS_BF_0");
    NUMARG(root_seed, "UTS_ROOT_SEED");
    NUMARG(tree_depth, "UTS_TREE_DEPTH");
    DBLARG(non_leaf_prob, "UTS_NON_LEAF_PROB");
    NUMARG(non_leaf_bf, "UTS_NON_LEAF_NUM");
    NUMARG(shift_depth, "UTS_SHIFT_DEPTH");
    NUMARG(num_samples, "UTS_NUM_SAMPLES");
    NUMARG(talk_to_self, "ALL_COMM");
    DBLARG(perc_remote, "PERC_REMOTE");

    // Tell Qthreads to initialize multinode support
    setenv("QT_MULTINODE", "yes", 1);

    qthread_f functions[4] = { visit, ping_comm, pong_comm, NULL };
    spr_init(SPR_SPMD, functions);

    // Set `num_locales` on each locale
    num_locales = spr_num_locales();
    here        = spr_locale_id();

#ifdef COLLECT_STATS
    comm_bins = calloc(num_locales, sizeof(aligned_t));
#endif

    // Collapse flow of control down to a single one, Chapel-style
    spr_unify();

    assert(here == 0);
    if (here != 0) {
        fprintf(stderr, "Unification failed!!!!!!!!!!!!!!!!!!\n");
        abort();
    }

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

    aligned_t ret = 0;
    qthread_fork(visit, &root, &ret);
    qthread_readFF(&total_num_nodes, &ret);

    qtimer_stop(timer);

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

#ifdef COLLECT_STATS
    num_messages = pong_comm(comm_bins);
    for (int i = 1; i < num_locales; i++) {
        aligned_t ret;
        qthread_fork_remote(ping_comm, &i, &ret, i, sizeof(int));

        aligned_t tmp;
        qthread_readFF(&tmp, &ret);
        num_messages += tmp;
    }

    free(comm_bins);
#endif /* ifdef COLLECT_STATS */

#ifdef PRINT_STATS
    iprintf("num-messages %lu\nperc-remote %f\ntree-size %lu\ntree-depth %d\nnum-leaves %llu\nperc-leaves %.2f\n",
            (unsigned long)num_messages,
            num_messages / (double)total_num_nodes,
            (unsigned long)total_num_nodes,
            (int)tree_height,
            (unsigned long long)num_leaves,
            num_leaves / (float)total_num_nodes * 100.0);
    iprintf("exec-time %.3f\ntotal-perf %.0f\npu-perf %.0f\n\n",
            total_time,
            total_num_nodes / total_time,
            total_num_nodes / total_time / qthread_num_workers());
#else /* ifdef PRINT_STATS */
    iprintf("Num messages = %lu, Tree size = %lu, tree depth = %d, num leaves = %llu (%.2f%%)\n",
            (unsigned long)num_messages,
            (unsigned long)total_num_nodes,
            (int)tree_height,
            (unsigned long long)num_leaves,
            num_leaves / (float)total_num_nodes * 100.0);
    iprintf("Wallclock time = %.3f sec, performance = %.0f "
            "nodes/sec (%.0f nodes/sec per PE)\n\n",
            total_time,
            total_num_nodes / total_time,
            total_num_nodes / total_time / qthread_num_workers());
#endif /* ifdef PRINT_STATS */

    spr_fini();

    return 0;
}

/* vim:set expandtab */
