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
#include "argparsing.h"

#include <tbb/task.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>

#define BRG_RNG // Select RNG
#include "../../utils/rng/rng.h"

#define PRINT_STATS 1

#define MAXNUMCHILDREN 100

using namespace tbb;

static unsigned long threads = task_scheduler_init::default_num_threads();

typedef enum {
    BIN = 0,
    GEO,
    HYBRID,
    BALANCED
} tree_t;
static const char *type_names[] = {
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
static const char *shape_names[] = {
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
    } else {
        if (num_children > MAXNUMCHILDREN) {
            printf("*** Number of children truncated from %d to %d\n",
                   num_children, MAXNUMCHILDREN);
            num_children = MAXNUMCHILDREN;
        }
    }

    return num_children;
}

// #define USE_REDUCTION 1

// Notes:
// -    Each task receives distinct copy of parent
// -    Copy of child is shallow, be careful with `state` member
class UTSvisit : public task
{
    node_t    parent;
    uint64_t *ret;
#ifdef USE_REDUCTION
    class UTSspawnChildren
    {
        node_t &parent;
public:
        uint64_t sum;
        UTSspawnChildren(node_t &_p) : parent(_p), sum(0) {}
        UTSspawnChildren(UTSspawnChildren &x,
                         split) : parent(x.parent), sum(0) {}
        void operator()(const blocked_range<int> &r)
        {
            node_t child;

            for (int i = r.begin(); i != r.end(); ++i) {
                uint64_t descendants;
                child.height = parent.height + 1;

                for (int j = 0; j < num_samples; j++) {
                    rng_spawn(parent.state.state, child.state.state, i);
                }

                child.num_children = calc_num_children(&child);

                UTSvisit &a = *new(task::allocate_root())UTSvisit(&child, &descendants);
                task::spawn_root_and_wait(a);
                sum += descendants;
            }
        }

        void join(const UTSspawnChildren &y)
        {
            sum += y.sum;
        }
    };
#endif // ifdef USE_REDUCTION
public:
    UTSvisit(node_t   *p,
             uint64_t *r) : parent(*p), ret(r) {}

    task *execute(void)
    {
        uint64_t num_descendants = 0;
        node_t   child;

        // Spawn children, if any
        if (parent.num_children > 0) {
#ifdef USE_REDUCTION
            UTSspawnChildren sc(parent);
            parallel_reduce(blocked_range<int>(0, parent.num_children, 1), sc);
            num_descendants += sc.sum;
#else
            uint64_t *child_descendants = new uint64_t[parent.num_children];

            set_ref_count(parent.num_children + 1);

            task_list list;
            for (int i = 0; i < parent.num_children; i++) {
                child.height = parent.height + 1;

                for (int j = 0; j < num_samples; j++) {
                    rng_spawn(parent.state.state, child.state.state, i);
                }

                child.num_children = calc_num_children(&child);

                // UTSvisit &a = *new(allocate_child())UTSvisit(&child, &child_descendants[i]);
                list.push_back(*new(allocate_child())UTSvisit(&child, &child_descendants[i]));
                // spawn(a);
            }

            spawn_and_wait_for_all(list);

            // Wait for children to finish up, accumulate descendants counts
            for (int i = 0; i < parent.num_children; i++) {
                num_descendants += child_descendants[i];
            }
            delete[] child_descendants;
#endif      // ifdef USE_REDUCTION
        }

        *ret = 1 + num_descendants;
        return NULL;
    }
};

#ifdef PRINT_STATS
static void print_stats(void)
{
    printf("tree-type %d\ntree-type-name %s\n",
           tree_type, type_names[tree_type]);
    printf("root-bf %.1f\nroot-seed %d\n",
           bf_0, root_seed);

    if ((tree_type == GEO) || (tree_type == HYBRID)) {
        printf("gen_mx %d\nshape-fn %d\nshape-fn-name %s\n",
               tree_depth, shape_fn, shape_names[shape_fn]);
    }

    if ((tree_type == BIN) || (tree_type == HYBRID)) {
        double q  = non_leaf_prob;
        int    m  = non_leaf_bf;
        double es = (1.0 / (1.0 - q * m));
        printf("q %f\nm %d\nE(n) %f\nE(s) %.2f\n",
               q, m, q * m, es);
    }

    if (tree_type == HYBRID) {
        printf("root-to-depth %d\n",
               (int)ceil(shift_depth * tree_depth));
    }

    if (tree_type == BALANCED) {
        printf("gen_mx %d\n", tree_depth);
        printf("expected-num-nodes %llu\nexpected-num-leaves %llu\n",
               (unsigned long long)((pow(bf_0, tree_depth + 1) - 1.0) / (bf_0 - 1.0)),
               (unsigned long long)pow(bf_0, tree_depth));
    }

    printf("compute-granularity %d\n", num_samples);
    printf("num-workers %lu\n", threads);

    printf("\n");

    fflush(stdout);
}

#else // ifdef PRINT_STATS
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
    printf("  Workers:   %d\n", threads);

    printf("\n");

    fflush(stdout);
}

#endif // ifdef PRINT_STATS

int main(int   argc,
         char *argv[])
{
    uint64_t total_num_nodes = 0;
    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    {
        unsigned long tmp;
        NUMARG(tmp, "UTS_TREE_TYPE");
        tree_type = (tree_t)tmp;
    }
    DBLARG(bf_0, "UTS_BF_0");
    NUMARG(root_seed, "UTS_ROOT_SEED");
    {
        unsigned long tmp;
        NUMARG(tmp, "UTS_SHAPE_FN");
        shape_fn = (shape_t)tmp;
    }
    NUMARG(tree_depth, "UTS_TREE_DEPTH");
    DBLARG(non_leaf_prob, "UTS_NON_LEAF_PROB");
    NUMARG(non_leaf_bf, "UTS_NON_LEAF_NUM");
    NUMARG(shift_depth, "UTS_SHIFT_DEPTH");
    NUMARG(num_samples, "UTS_NUM_SAMPLES");

    NUMARG(threads, "TBB_THREADS");

#ifdef PRINT_STATS
    print_stats();
#else
    print_banner();
#endif
    {
        tbb::task_scheduler_init init(threads);

        timer = qtimer_create();
        qtimer_start(timer);

        node_t root;
        root.height = 0;
        rng_init(root.state.state, root_seed);
        root.num_children = calc_num_children(&root);

        UTSvisit &a = *new(task::allocate_root())UTSvisit(&root, &total_num_nodes);
        task::spawn_root_and_wait(a);

        qtimer_stop(timer);
    }

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

#ifdef PRINT_STATS
    printf("tree-size %lu\ntree-depth %d\nnum-leaves %llu\nperc-leaves %.2f\n",
           (unsigned long)total_num_nodes,
           (int)tree_height,
           (unsigned long long)num_leaves,
           num_leaves / (float)total_num_nodes * 100.0);
    printf("exec-time %.3f\ntotal-perf %.0f\npu-perf %.0f\n\n",
           total_time,
           total_num_nodes / total_time,
           total_num_nodes / total_time / threads);
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
           total_num_nodes / total_time / threads);
#endif // ifdef PRINT_STATS

    return 0;
}

/* vim:set expandtab */
