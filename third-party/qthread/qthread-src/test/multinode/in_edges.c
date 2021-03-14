#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* for INT_MAX */
#include <math.h>   /* for floor, log, sin */
#include <time.h>   /* for time */
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include <qthread/spr.h>
#include <qthread/multinode.h>
#include "argparsing.h"

#define BRG_RNG // Select RNG
#include "../utils/rng/rng.h"

#define NUM_VERTICES 4

typedef size_t vertex_id_t;

typedef struct {
    size_t      lid;
    vertex_id_t vid;
} vertex_t;

static struct state_t rng_state;
static int            here;
static size_t         num_locales;
static vertex_id_t    indices[NUM_VERTICES + 1];
static vertex_id_t    in_degrees[NUM_VERTICES];

size_t random_locale(void)
{
    int r = rng_nextrand(rng_state.state);

    return (size_t)(num_locales * ((double)r / (double)INT_MAX));
}

size_t random_vertex(void)
{
    int r = rng_nextrand(rng_state.state);
    
    return (size_t)(NUM_VERTICES * ((double)r / (double)INT_MAX));
}

aligned_t incr_in_degree(void * args_)
{
    vertex_id_t src = *((vertex_id_t *)args_);

    printf("[%03d] incrementing in-degree at %p\n", here, &(indices[src]));
    qthread_incr(&(in_degrees[src]), 1);

    return 0;
}

int main(int   argc,
         char *argv[])
{
    size_t        num_edges;
    aligned_t   * rets;
    vertex_t    * edges;

    for (int i = 0; i < NUM_VERTICES; i++) {
        in_degrees[i] = 0;
    }
    /* Initialize SPR in SPMD mode */
    qthread_f actions[2] = {incr_in_degree, NULL};
    spr_init(SPR_SPMD, actions);
    here        = spr_locale_id();
    num_locales = spr_num_locales();
    if (0 == here) {
        printf("Running with %d locales\n", num_locales);
    }

    rng_init(rng_state.state, time(NULL) * here);

    /* Create local portion of the graph */
    indices[0] = 0;
    for (int i = 1; i < NUM_VERTICES + 1; i++) {
        indices[i]    = indices[i-1] + random_vertex();
    }
    for (int i = 0; i < NUM_VERTICES + 1; i++) {
        printf("[%03d] indices[%d]: %lu\n", here, i, indices[i]);
    }

    num_edges = indices[NUM_VERTICES];
    edges = malloc(num_edges * sizeof(vertex_t));
    for (int i = 0; i < num_edges; i++) {
        edges[i].lid = random_locale();
        edges[i].vid = random_vertex();
    }

    for (int i = 0; i < num_edges; i++) {
        printf("[%03d] edges[%d]: (%lu,%lu)\n", here, i, 
               edges[i].lid, edges[i].vid);
    }

    qt_global_barrier();

    /* Fill in-degrees property map */
    rets = malloc(num_edges * sizeof(aligned_t));
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = indices[i]; j < indices[i+1]; j++) {
            printf("[%03d] spawning incr of edge[%d] = (%lu,%lu)\n",
                   here, j, edges[j].lid, edges[j].vid);
            qthread_fork_remote(incr_in_degree,         /* action */
                                &(edges[j].vid),        /* local vertex id */
                                &rets[j],               /* feb */
                                edges[j].lid,           /* locale */
                                sizeof(vertex_id_t));
        }
    }
    for (int i = 0; i < num_edges; i++) {
        qthread_readFF(&rets[i], &rets[i]);
    }

    /* Print in-degrees */
    for (int i = 0; i < NUM_VERTICES; i++) {
        printf("[%03d] in-degree(%lu) = %lu\n",
               here, i, in_degrees[i]);
    }

    /* Free up allocated resources */
    free(rets);
    free(edges);

    return 0;
}

/* vim:set expandtab */
